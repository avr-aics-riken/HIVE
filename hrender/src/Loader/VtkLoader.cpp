/**
 * @file SphLoader.cpp
 * VTKデータローダー
 */
#include <stdio.h>
#include <string.h>
#include "VtkLoader.h"
#include "tiny_vtkxml.h"
#include "BufferVolumeData.h"

#include <cassert>

#ifdef _OPENMP
#include <omp.h>
#endif

namespace {

typedef struct {
    std::string type; 
    std::string name;
    int numberOfComponents;
    int length;
    std::vector<unsigned char> data; // actual data
    size_t offset;
} DataInfo;

bool LoadImageDataFile(const char* filename)
{
    std::vector<DataInfo> dataList;

    // Parse VTK XML and find RAW data position/offset.
    {
        lte::tinyvtkxml::TinyVTKXML toplevelXML(filename);
        if (toplevelXML.Parse()) {
            fprintf(stderr, "[VTKLoader] Parsing failed. %s\n", filename);
            return false;
        }

        lte::tinyvtkxml::Node* root = toplevelXML.GetRoot();
        if (!root) {
            fprintf(stderr, "[VTKLoader] Root node not found.\n");
            return false;
        }
            
        if (!root->HasAttribute("type")) {
            fprintf(stderr, "[VTKLoader] Root node must have \"type\" attribute.\n");
            return false;
        }

        bool isBigEndian = false;
        if (root->HasAttribute("byte_order")) {
            isBigEndian = (root->GetAttribute("byte_order") == "BigEndian") ? true : false;
        }

        // @todo { Read Extent and check its value with parent XML node}
        std::vector<lte::tinyvtkxml::Node*> dataArrays = root
            ->FindChild("ImageData")
            ->FindChild("Piece")
            ->FindChild("CellData")
            ->FindChildren("DataArray");

        if (dataArrays.size() == 0) {
            return false;
        }

        for (int i = 0; i < dataArrays.size(); ++i) {
            std::string type = "";
            std::string name = "";
            std::string format;
            size_t offset = 0;

            if (dataArrays[i]->HasAttribute("type")) {
                type = dataArrays[i]->GetAttribute("type");
            }
            if (type != "Float32") {
                fprintf(stderr, "Currently only \"Float32\" is supported for \"type\" in DataArray.\n");
                return false;
            }

            if (dataArrays[i]->HasAttribute("Name")) {
                name = dataArrays[i]->GetAttribute("Name");
            }

            if (dataArrays[i]->HasAttribute("format")) {
                format = dataArrays[i]->GetAttribute("format");
            }
            if (format != "appended") {
                fprintf(stderr, "\"appended\" should be specified for \"format\" attribute.\n");
                return false;
            }
            
            if (dataArrays[i]->HasAttribute("offset")) {
                offset = dataArrays[i]->GetAttributeInt("offset");
            }

            // Assume binary(RAW) data.
            // raw = '_' + datalen(4 byte) + data
            //
            // offset value contains datalen(4byte).
            size_t position = dataArrays[i]->GetPosition(); // position from the beginning of file
            size_t length = dataArrays[i]->GetLength();
            DataInfo info;
            info.offset = position + 1 + sizeof(int); // skip '_' and datalen header
            info.length = length;
            info.name = name;
            info.type = type;
            info.numberOfComponents = 0; // @fixme
            dataList.push_back(info);
        }
    }

    if (dataList.empty()) {
        return false;
    }

    // Read actual data.
    FILE* fp = fopen(filename, "rb");

    for (size_t i = 0; i < dataList.size(); i++) {
        fseek(fp, dataList[i].offset, SEEK_SET);
        dataList[i].data.resize(dataList[i].length);
        size_t n = fread(&(dataList[i].data.at(0)), 1, dataList[i].length, fp);
        assert(n == dataList[i].length);
    }

    fclose(fp);

    return true;
}

} // namespace

/// コンストラクタ
VTKLoader::VTKLoader()
{
    Clear();
}

/// デストラクタ
VTKLoader::~VTKLoader()
{
    Clear();
}

/// メンバクリア
void VTKLoader::Clear()
{
    m_volume = 0;
}

/**
 * VTKデータのロード
 * @param filename ファイルパス
 * @retval true 成功
 * @retval false 失敗
 */
bool VTKLoader::Load(const char* filename)
{
    Clear();
    m_volume = new BufferVolumeData();

    lte::tinyvtkxml::TinyVTKXML toplevelXML(filename);
    if (toplevelXML.Parse()) {
        fprintf(stderr, "[VTKLoader] Parsing failed. %s\n", filename);
        return false;
    }

    lte::tinyvtkxml::Node* root = toplevelXML.GetRoot();
    if (!root) {
        fprintf(stderr, "[VTKLoader] Root node not found.\n");
        return false;
    }
        
    if (!root->HasAttribute("type")) {
        fprintf(stderr, "[VTKLoader] Root node must have \"type\" attribute.\n");
        return false;
    }

    std::string type = root->GetAttribute("type");

    if (type == "PImageData") { // Piece ImageData VTK file
        lte::tinyvtkxml::Node* nodePImageData = root->FindChild("PImageData");
        if (!nodePImageData) {
            fprintf(stderr, "[VTKLoader] \"PImageData\" tag not found.\n");
            return false;
        }

        int wholeExtent[6] = {0, 0, 0, 0, 0, 0};
        double origin[3] = {0.0, 0.0, 0.0};
        double spacing[3] = {0.0, 0.0, 0.0};
        if (!nodePImageData->HasAttribute("WholeExtent")) {
            fprintf(stderr, "[VTKLoader] \"PImageData\" tag must have \"WholeExtent\" attribute.\n");
            return false;
        }

        sscanf(nodePImageData->GetAttribute("WholeExtent").c_str(), "%d %d %d %d %d %d",
            &wholeExtent[0], &wholeExtent[1], &wholeExtent[2],
            &wholeExtent[3], &wholeExtent[4], &wholeExtent[5]);
         
        int width = wholeExtent[1];
        int height = wholeExtent[3];
        int depth = wholeExtent[5];

        // Parse attribute list.
        m_dataArrayInfoList.clear();
        {
            std::vector<lte::tinyvtkxml::Node*> pdataArrays = nodePImageData->FindChild("PCellData")->FindChildren("PDataArray");
            for (size_t i = 0; i < pdataArrays.size(); i++) {
                if (pdataArrays[i]->HasAttribute("type") && pdataArrays[i]->HasAttribute("Name")) {
                    std::string type = pdataArrays[i]->GetAttribute("type");
                    std::string name = pdataArrays[i]->GetAttribute("Name");
                    int numberOfComponents = 1;
                    if (pdataArrays[i]->HasAttribute("NumberOfComponents")) {
                        numberOfComponents = pdataArrays[i]->GetAttributeInt("NumberOfComponents");
                    }
                    //printf("ty: %s, nm: %s, c: %d\n", type.c_str(), name.c_str(), numberOfComponents);
                    DataArrayInfo info;
                    info.type = type;
                    info.name = name;
                    info.numberOfComponents = numberOfComponents;
                    m_dataArrayInfoList.push_back(info);
                }
            }
        }

        // Parse Piece tag like this:
        // <Piece Extent="0 32 0 32 0 32" Source="./0000180100/data-flow-00000-0000180100.vti"/>
        {
            std::vector<lte::tinyvtkxml::Node*> pieceArrays = nodePImageData->FindChildren("Piece");
            for (size_t i = 0; i < pieceArrays.size(); i++) {
                if (pieceArrays[i]->HasAttribute("Extent") && pieceArrays[i]->HasAttribute("Source")) {
                    std::string extent = pieceArrays[i]->GetAttribute("Extent");
                    std::string source = pieceArrays[i]->GetAttribute("Source");
                    int vals[6];
                    int n = sscanf(extent.c_str(), "%d %d %d %d %d %d", &vals[0], &vals[1], &vals[2], &vals[3], &vals[4], &vals[5]);
                    if (n != 6) {
                        fprintf(stderr, "[VTKLoader] Invalid \"Extent\" attribute.\n");
                        continue;
                    }

                    PieceInfo info;
                    for (int k = 0; k < 6; k++) {
                        info.extent[k] = vals[k];
                    }
                    info.source = source;

                    m_pieceInfoList.push_back(info);
                }
            }
        }
            
        // Load piece ImageData files
        #ifdef OPENMP
        #pragma omp parallel for
        #endif
        for (size_t i = 0; i < m_pieceInfoList.size(); i++) {
            printf("reading piece[%d]\n", i);
            bool ret = LoadImageDataFile(m_pieceInfoList[i].source.c_str());
        }

        
    } else if (type == "ImageData") { // ImageData VTK file
        printf("ImageDataNode found\n");
        bool ret = LoadImageDataFile(filename);
    } else {
        fprintf(stderr, "[VTKLoader] Unsupported VTK type. \"PImageData\" or \"ImageData\" expected.\n");
        return false;
    }


	// @todo
	return false;
}

/**
 * VTKWidth取得
 * @retval int Width
 */
int VTKLoader::Width()    {
    return m_volume->Width();
}

/**
 * VTKHeight取得
 * @retval int Height
 */
int VTKLoader::Height()   {
    return m_volume->Height();
}

/**
 * VTKDepth取得
 * @retval int Depth
 */
int VTKLoader::Depth()    {
    return m_volume->Depth();
}

/**
 * VTKComponent取得
 * @retval int Component数
 */
int VTKLoader::Component() {
    return m_volume->Component();
}

/**
 * VTKデータバッファ参照取得
 * @retval FloatBuffer* FloatBufferアドレス
 */
FloatBuffer* VTKLoader::Buffer() {
    return m_volume->Buffer();
}

/**
 * VolumeData参照取得
 * @retval BufferVolumeData* VolumeData参照
 */
BufferVolumeData *VTKLoader::VolumeData()
{
    return m_volume;
}
