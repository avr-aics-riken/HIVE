/**
 * @file PdmLoader.cpp
 * PDMデータローダー
 */
#include "PdmLoader.h"

#ifndef HIVE_WITH_PDMLIB
#error "HIVE_WITH_PDMLIB must be defined when you compile CDMLoader module"
#endif

#include <cstring>

#include "PDMlib.h"
#include "../Buffer/Buffer.h"

/// コンストラクタ
PDMLoader::PDMLoader(){}
/// デストラクタ
PDMLoader::~PDMLoader(){};

/// メンバクリア
void PDMLoader::Clear()
{
	points.Clear();
}

/**
 * PDMデータのロード
 * @param filename ファイルパス
 * @param timeStep time step
 * @param readlAll read all data? true:for data conversion, false: each MPI rank reads each paticle data.
 * @retval true 成功
 * @retval false 失敗
 */
bool PDMLoader::Load(const char* filename, int timeStep){
	Clear();


	printf("CDM load\n");
	std::string fname = std::string(filename);

	// Its OK to pass argc=0,argv=NULL since its only used to initialize MPI inside of Zoltan lib(called from PDMlib::Init()).
	// We alreadly initialized MPI, thus argc and argv will not be used.
	PDMlib::PDMlib::GetInstance().Init(0, NULL, /* write filename = */"",  fname);

	std::vector<PDMlib::ContainerInfo>& infos = PDMlib::PDMlib::GetInstance().GetContainerInfo();
	printf("# of infos = %d\n", infos.size());
	for (size_t i = 0; i < infos.size(); i++) {
		printf("[%d] name = %s, type = %d, ncomp = %d, order = %d, suffix = %s, annotation = %s, compression = %s\n",
				i, infos[i].Name.c_str(), infos[i].Type, infos[i].nComp, infos[i].VectorOrder, infos[i].Suffix.c_str(), infos[i].Annotation.c_str(), infos[i].Compression.c_str());
	}

	int* ID = NULL;
	float* Coordinate = NULL;

	//PDMlib::PDMlib::GetInstance().RegisterContainer("ParticleID", &ID);
	//PDMlib::PDMlib::GetInstance().RegisterContainer("Coordinate", &Coordinate);

	size_t CoordinateLen = 0;
	bool readall = false; // @fixme
	{
		int ret = PDMlib::PDMlib::GetInstance().Read("Coordinate", &CoordinateLen, &Coordinate, &timeStep, readall);
	}

	// Alloc
	{
		printf("[PDMlib] CoorinateLen = %d\n", CoordinateLen);
		points.Create(CoordinateLen);

		std::copy(Coordinate, Coordinate + CoordinateLen, points.Position()->GetBuffer());
	}

	delete [] Coordinate;

	// @todo { Read custom attributes. }

	// @todo {}
	return false;
}

/**
 * 点データ取得
 * @retval BufferPointData* 点データバッファへの参照
 */
BufferPointData *PDMLoader::PointData()
{
	return &points;
}
