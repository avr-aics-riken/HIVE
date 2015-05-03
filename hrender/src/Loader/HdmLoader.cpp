/**
 * @file HdmLoader.cpp
 * HDMデータローダー
 */


#ifndef HIVE_WITH_HDMLIB
#error "HIVE_WITH_HDMLIB must be defined when you compile HDMLoader module"
#endif

#include <mpi.h> // must include mpi.h befor stdio.h for Intel MPI compiler.

#include <stdio.h>
#include <string.h>

#include <string>

#include "HdmLoader.h"
#include "SimpleVOL.h"

#include "BlockFactory.h"
#include "Block.h"
#include "BlockManager.h"
#include "Scalar3D.h"

#include "BCMTools.h"
#include "BoundaryConditionSetterBase.h"
#include "BCMOctree.h"
#include "BoundaryInfo.h"


//#include "LeafBlockLoader.h"
#include "BCMFileLoader.h"
#include "BCMFileSaver.h"

/// コンストラクタ
HDMLoader::HDMLoader()
{
    Clear();
}

/// デストラクタ
HDMLoader::~HDMLoader()
{
    Clear();
}

/// ボリュームクリア
void HDMLoader::Clear()
{
    m_volume.Clear();
}

namespace {

typedef enum {
	TYPE_UNKNOWN,
	TYPE_FLOAT32,
	TYPE_FLOAT64,
} DataType;

DataType ParseDataType(const std::string& type)
{
	if (type == "Float32") {
		return TYPE_FLOAT32;
	} else if (type == "Float64") {
		return TYPE_FLOAT64;
	}

	return TYPE_UNKNOWN;
}

// From HDMlib example.
class BoundaryConditionSetter : public BoundaryConditionSetterBase {

public:

  BoundaryConditionSetter() {}

  ~BoundaryConditionSetter() {}

  BoundaryInfo* operator() (const Node* node, const BCMOctree* tree) {
    BoundaryInfo* boundaryInfo = new BoundaryInfo[NUM_FACE];

    // Set boundary condition
    for (int i = 0; i < NUM_FACE; i++) {
      Face face = Face(i);
      if (tree->checkOnOuterBoundary(node, face)) {
        boundaryInfo[face].setType(BoundaryInfo::DIRICHLET);
        boundaryInfo[face].setID(0);
      }
    }

    return boundaryInfo;
  }

};

template <typename T>
void print(const char* prefix, const int dcid, const int vc = 0){
    BlockManager& blockManager = BlockManager::getInstance();
    const MPI::Intracomm& comm = blockManager.getCommunicator();
    Vec3i sz = blockManager.getSize();

    char name[128]; sprintf(name, "%s_%03d.txt", prefix, comm.Get_rank());
    FILE *fp = fopen(name, "w");
    for(int id = 0; id < blockManager.getNumBlock(); ++id){
        BlockBase* block = blockManager.getBlock(id);

        Scalar3D<T> *mesh = dynamic_cast< Scalar3D<T>* >(block->getDataClass(dcid));
        T* data = mesh->getData();
        Index3DS idx = mesh->getIndex();

        fprintf(fp, "Data[%3d] (vc : %d) \n", id, vc);
        for(int z = -vc; z < sz.z + vc; z++){
            for(int y = -vc; y < sz.y + vc; y++){
                for(int x = -vc; x < sz.x + vc; x++){
                    fprintf(fp, "%g ", data[idx(x, y, z)]);
                }
                fprintf(fp, "\n");
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
	printf("dbg: output %s\n", name);
}

}

/**
 * HDMデータのロード
 * @param cellidFilename Cell ID ファイルパス
 * @param dataFilename   data ファイルパス
 * @param fieldName      field name
 * @param fieldType      field type("Float32", "Float64", etc)
 * @param components     The number of components(1 = scalar, 3 = vector)
 * @param virtualCells   The number of virtual cells(Usually 2)
 * @retval true 成功
 * @retval false 失敗
 */
bool HDMLoader::Load(const char* cellidFilename, const char* dataFilename, const char* fieldName, const char* fieldType, int components, int virtualCells)
{
    Clear();

	if (components == 1 || components == 3) {
		// OK
	} else {
		fprintf(stderr, "[HDMLoader] components must be 1 or 3, but %d given\n", components);
	    return false;
	}

	DataType type = ParseDataType(fieldType);
	if (type == TYPE_UNKNOWN) {
		fprintf(stderr, "[HDMLoader] Unsupported field type: %s\n", fieldType);
	    return false;
	}

	// cellid.bcm
	BoundaryConditionSetter* bcsetter = new BoundaryConditionSetter;
	BCMFileIO::BCMFileLoader loader(cellidFilename, bcsetter);
	delete bcsetter;

	BlockManager& blockManager = BlockManager::getInstance();
    blockManager.printBlockLayoutInfo();

	// data.bcm
	if( !loader.LoadAdditionalIndex(dataFilename) ){
		fprintf(stderr, "[HDMLoader] Load File Error %s\n", dataFilename);
	    return false;
	}

	// Get timestep for given field
	const BCMFileIO::IdxStep* step = loader.GetStep(fieldName);

	const std::list<unsigned int>* stepList = step->GetStepList();
	printf("dbg: # of steps = %d\n", stepList->size());

	int vc = virtualCells;

  	int id_cid = 0;
    int id_s32 = 0;
    int id_s64 = 0;
    int id_v32[3] = {0}; // 0:u, 1:v, 2:w
    int id_v64[3] = {0}; // 0:u, 1:v, 2:w

	loader.LoadLeafBlock(&id_cid, "CellID", vc);

	for (std::list<unsigned int>::const_iterator it = stepList->begin(); it != stepList->end(); it++) {

		char prefix[1024];
		sprintf(prefix, "%s_%03d", fieldName, *it);

		if (type == TYPE_FLOAT32) {

			if (components == 1) {
				loader.LoadLeafBlock(&id_s32, fieldName, vc, *it);
				print<float>(prefix, id_s32, vc);
			} else {
				loader.LoadLeafBlock(id_v32, fieldName, vc, *it);
			}

		} else if (type == TYPE_FLOAT64) {
			if (components == 1) {
				loader.LoadLeafBlock(&id_s64, fieldName, vc, *it);
				print<float>(prefix, id_s64, vc);
			} else {
				loader.LoadLeafBlock(id_v64, fieldName, vc, *it);
			}
		}

		// @todo { Construct volume data. }

	}

	return false;
}

/**
 * HDMWidth取得
 * @retval int HDMWidth
 */
int HDMLoader::Width()    {
    return m_volume.Width();
}

/**
 * HDMHeight取得
 * @retval int HDMHeight
 */
int HDMLoader::Height()   {
    return m_volume.Height();
}

/**
 * HDMDepth取得
 * @retval int HDMDepth
 */
int HDMLoader::Depth()    {
    return m_volume.Depth();
}

/**
 * HDMComponent取得
 * @retval int Component数
 */
int HDMLoader::Component() {
    return m_volume.Component();
}

/**
 * HDMデータバッファ取得
 * @retval FloatBuffer* HDMデータバッファアドレス
 */
FloatBuffer* HDMLoader::Buffer() {
    return m_volume.Buffer();
}

/**
 * VolumeData取得
 * @retval VolumeData* VolumeDataアドレス
 */
BufferVolumeData *HDMLoader::VolumeData()
{
    return &m_volume;
}

