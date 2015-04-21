/**
 * @file SphLoader.cpp
 * VTKデータローダー
 * @todo { Field inspector. Support varyious data type other than float32. Load data as sparse voxel representation. }
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

inline size_t IDX(size_t comps, size_t x, size_t y, size_t z, size_t c, size_t sx, size_t sy, size_t sz) {
    size_t dx = (std::max)((size_t)0, (std::min)(sx-1, x));
    size_t dy = (std::max)((size_t)0, (std::min)(sy-1, y));
    size_t dz = (std::max)((size_t)0, (std::min)(sz-1, z));
    size_t idx = comps * (dz * sx * sy + dy * sx + dx) + c;
    return idx;
}

template<typename T>
inline float D(const T* data, size_t comps, size_t x, size_t y, size_t z, size_t c, size_t sx, size_t sy, size_t sz) {
    size_t dx = (std::max)((size_t)0, (std::min)(sx-1, x));
    size_t dy = (std::max)((size_t)0, (std::min)(sy-1, y));
    size_t dz = (std::max)((size_t)0, (std::min)(sz-1, z));
    size_t idx = comps * (dz * sx * sy + dy * sx + dx) + c;
    return data[idx];
}

typedef struct {
    std::string type; 
    std::string name;
    int numberOfComponents;
    int length;
    std::vector<unsigned char> data; // actual data
    size_t offset;
} DataInfo;

bool LoadPImageDataFile(
    float* volume,  // [inout]
    const std::string& filename,
    const VTKLoader::PieceInfo& pieceInfo,
    const std::string& fieldName,
    const size_t dim[3]) // global dim
{
    std::vector<DataInfo> dataList;

    // Parse VTK XML and find RAW data position/offset.
    {
        lte::tinyvtkxml::TinyVTKXML toplevelXML(filename.c_str());
        if (toplevelXML.Parse()) {
            fprintf(stderr, "[VTKLoader] Parsing failed. %s\n", filename.c_str());
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

        // @todo { Read Extent and check its value with piece information from parent XML node}
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
            int numberOfComponents = 1;

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

            if (dataArrays[i]->HasAttribute("NumberOfComponents")) {
                numberOfComponents = dataArrays[i]->GetAttributeInt("NumberOfComponents");
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
            info.numberOfComponents = numberOfComponents;
            dataList.push_back(info);
        }
    }

    if (dataList.empty()) {
        return false;
    }

    // Read actual data.
    FILE* fp = fopen(filename.c_str(), "rb");

    for (size_t i = 0; i < dataList.size(); i++) {
        fseek(fp, dataList[i].offset, SEEK_SET);
        dataList[i].data.resize(dataList[i].length);
        size_t n = fread(&(dataList[i].data.at(0)), 1, dataList[i].length, fp);
        assert(n == dataList[i].length);
    }

    fclose(fp);

    size_t size[3];
    size_t offset[3];
    size[0] = pieceInfo.extent[1] - pieceInfo.extent[0];
    size[1] = pieceInfo.extent[3] - pieceInfo.extent[2];
    size[2] = pieceInfo.extent[5] - pieceInfo.extent[4];
    offset[0] = pieceInfo.extent[0];
    offset[1] = pieceInfo.extent[2];
    offset[2] = pieceInfo.extent[4];

    // Find field
    int fieldIdx = -1;
    {
        for (size_t i = 0; i < dataList.size(); i++) {
            if (dataList[i].name == fieldName) {
                fieldIdx = i;
                break;
            }
        }
    }

    assert(pieceInfo.numberOfComponents == dataList[fieldIdx].numberOfComponents);

    // Fill voxel
    {
        int components = dataList[fieldIdx].numberOfComponents;
        for (size_t z = 0; z < size[2]; z++) {
            for (size_t y = 0; y < size[1]; y++) {
                for (size_t x = 0; x < size[0]; x++) {
                    for (size_t c = 0; c < components; c++) {
                        const float* src = reinterpret_cast<const float*>(&(dataList[fieldIdx].data.at(0)));
                        volume[IDX(components, x + offset[0], y + offset[1], z + offset[2], c, dim[0], dim[1], dim[2])] = src[IDX(components, x, y, z, c, size[0], size[1], size[2])];
                    }
                }
            }
        }
    }

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
 * @param fieldname field name
 * @retval true 成功
 * @retval false 失敗
 */
bool VTKLoader::Load(const char* filename, const char* fieldname)
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
         
        size_t globalDim[3];

        globalDim[0] = wholeExtent[1];
        globalDim[1] = wholeExtent[3];
        globalDim[2] = wholeExtent[5];

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

        // Find fieldname
        int fieldIdx = -1;
        {
            for (size_t i = 0; i < m_dataArrayInfoList.size(); i++) {
                if (m_dataArrayInfoList[i].name == std::string(fieldname)) {
                    fieldIdx = i; // got it
                    break;
                }
            }
        }

        if (fieldIdx < 0) {
            fprintf(stderr, "[VTKLoader] field name \"%s\" not found.\n", fieldname);
            return false;
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

        // Prepare volume data.
        {
            m_volume->Create(globalDim[0], globalDim[1], globalDim[2], m_dataArrayInfoList[fieldIdx].numberOfComponents);
            m_origin[0] = origin[0];
            m_origin[1] = origin[1];
            m_origin[2] = origin[2];
            m_pitch[0] = spacing[0];
            m_pitch[1] = spacing[1];
            m_pitch[2] = spacing[2];
        }
            
        bool ok = true;
        // Load piece ImageData files
        #ifdef OPENMP
        #pragma omp parallel for shared(ok)
        #endif
        for (size_t i = 0; i < m_pieceInfoList.size(); i++) {
            //printf("reading piece[%d]\n", i);
            bool ret = LoadPImageDataFile(m_volume->Buffer()->GetBuffer(), m_pieceInfoList[i].source, m_pieceInfoList[i], fieldname, globalDim);
            if (!ret) {
                ok = false; // Not using atomic op is probably OK for atomically updating bool value.
            }
        }

        return ok;
        
    } else if (type == "ImageData") { // ImageData VTK file
        printf("ImageDataNode found\n");
        // @todo
        assert(0);
        //bool ret = LoadImageDataFile(std::string(filename));
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
