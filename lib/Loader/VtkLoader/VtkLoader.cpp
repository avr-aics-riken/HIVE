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
#include <wordexp.h>
#include <fstream>

#ifdef _OPENMP
#include <omp.h>
#endif

namespace {

bool IsBigEndian(void) {
	union {
		unsigned int i;
		char c[4];
	} bint = {0x01020304};

	return bint.c[0] == 1;
}


inline void swap4(unsigned int *val) {
	unsigned int tmp = *val;
	unsigned char *dst = (unsigned char *)val;
	unsigned char *src = (unsigned char *)&tmp;

	dst[0] = src[3];
	dst[1] = src[2];
	dst[2] = src[1];
	dst[3] = src[0];
}

inline size_t IDX(ssize_t comps, ssize_t x, ssize_t y, ssize_t z, ssize_t c, ssize_t sx, ssize_t sy, ssize_t sz) {
    ssize_t dx = (std::max)((ssize_t)0, (std::min)(sx-1, x));
    ssize_t dy = (std::max)((ssize_t)0, (std::min)(sy-1, y));
    ssize_t dz = (std::max)((ssize_t)0, (std::min)(sz-1, z));
    ssize_t idx = comps * (dz * sx * sy + dy * sx + dx) + c;
    return idx;
}

template<typename T>
inline float D(const T* data, ssize_t comps, ssize_t x, ssize_t y, ssize_t z, ssize_t c, ssize_t sx, ssize_t sy, ssize_t sz) {
    return data[IDX(comps, x, y, z, c, sx, sy, sz)];
}

std::vector<std::string> SplitPath(const std::string& s, char c) {
  if (s.empty()) {
    return std::vector<std::string>();
  }
 
  std::vector<std::string> res;
  int begin = 0, end = 0;
  for (int i = 0; i < s.size(); ++i) {
    if (s[i] == c) {
      res.push_back(s.substr(begin, end - begin));
      begin = i + 1;
      end = i + 1;
    } else {
      ++end;
    }
  }
 
  res.push_back(s.substr(begin));
 
  return res;
}
  
std::string ExpandFilePath(const std::string& filepath)
{
#ifdef _WIN32
  // @todo {}
  DWORD len = ExpandEnvironmentStringsA(filepath.c_str(), NULL, 0 );
  char* str = new char [len];
  ExpandEnvironmentStringsA(filepath.c_str(), str, len );

  std::string s(str);

  delete [] str;

  return s;
#else
  
  std::string s;
  wordexp_t p;

  if (filepath.empty()) {
    return "";
  }

  //char** w;
  int ret = wordexp( filepath.c_str(), &p, 0 );
  if (ret) {
    fprintf(stderr, "Filepath expansion err: %d\n", ret);
    // err
    s = filepath;
    return s;
  }

  // Use first element only.
  if (p.we_wordv) {
    s = std::string(p.we_wordv[0]);
    wordfree( &p );
  } else {
    s = filepath;
  }

  return s;
#endif
}


std::string JoinPath(const std::string& path0, const std::string& path1)
{
  if (path0.empty()) {
    return path1;
  } else {
    // check '/'
    char lastChar = *path0.rbegin();
    if (lastChar != '/') {
      return path0 + std::string("/") + path1;
    } else {
      return path0 + path1;
    }
  }
}

bool FileExists(const std::string& abs_filename)
{
    bool ret;
    std::ifstream ifs(abs_filename.c_str());
    if (ifs.fail()) {
        return false;
    }

    return true;
}

std::string FindFile(const std::vector<std::string>& paths, const std::string& filepath)
{
  for (size_t i = 0; i < paths.size(); i++) {
    std::string absPath = ExpandFilePath(JoinPath(paths[i], filepath));
    if (FileExists(absPath)) {
      return absPath;
    }
  }

  return std::string();
}

typedef struct {
    std::string type; 
    std::string name;
    int numberOfComponents;
    int length;
    std::vector<unsigned char> data; // actual data
    size_t offset;
} DataInfo;

bool LoadImageDataFile(
    float* volume,  // [inout]
    const std::string& filename,
    const VTKLoader::PieceInfo& pieceInfo,
    const std::string& fieldName,
	bool doByteSwap,
	int numberOfComponents,
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

        bool isDataBigEndian = false;
        if (root->HasAttribute("byte_order")) {
            isDataBigEndian = (root->GetAttribute("byte_order") == "BigEndian") ? true : false;
        }

        bool isArchBigEndian = IsBigEndian();

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
            info.offset = position + sizeof(int); // skip datalen(4 bytes)
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
        //fseek(fp, dataList[i].offset, SEEK_SET);
		//int dataLen;
		//fread(&dataLen, 1, 4, fp);
		//swap4(reinterpret_cast<unsigned int*>(&dataLen));
		//printf("offt = %d\n", dataList[i].offset);
		//printf("n = %d, datalen = %d\n", dataLen, dataList[i].length);
		//assert(dataLen == dataList[i].length);

        fseek(fp, dataList[i].offset, SEEK_SET);
        dataList[i].data.resize(dataList[i].length);
        size_t n = fread(&(dataList[i].data.at(0)), 1, dataList[i].length, fp);

		if (doByteSwap) {
			// Assume float data.
			for (size_t c = 0; c < dataList[i].length / sizeof(float); c++) {
				swap4(reinterpret_cast<unsigned int*>(&dataList[i].data[sizeof(float)*c]));
			}
		}
        assert(n == dataList[i].length);

		for (size_t c = 0; c < dataList[i].length / sizeof(float); c++) {
			//printf("
			//	swap4(reinterpret_cast<unsigned int*>(&dataList[i].data[sizeof(float)*c]));
		}
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

	//printf("off: %d, %d, %d, sz: %d, %d, %d\n", offset[0], offset[1], offset[2], size[0], size[1], size[2]);

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

    assert(numberOfComponents == dataList[fieldIdx].numberOfComponents);

    // Fill voxel
    {
        int components = dataList[fieldIdx].numberOfComponents;
        for (size_t z = 0; z < size[2]; z++) {
            for (size_t y = 0; y < size[1]; y++) {
                for (size_t x = 0; x < size[0]; x++) {
                    for (size_t c = 0; c < components; c++) {
                        const float* src = reinterpret_cast<const float*>(&(dataList[fieldIdx].data.at(0)));
                        volume[IDX(components, x + offset[0], y + offset[1], z + offset[2], c, dim[0], dim[1], dim[2])] = src[IDX(components, x, y, z, c, size[0], size[1], size[2])];
						//if (src[IDX(components, x, y, z, c, size[0], size[1], size[2])] > 0) {
						//	printf("val = %f\n", src[IDX(components, x, y, z, c, size[0], size[1], size[2])]);
						//} 
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
 * @param searchpath search パス(optional)
 * @param fieldname field name
 * @param doByteSwap apply byteswap for input data?
 * @retval true 成功
 * @retval false 失敗
 */
bool VTKLoader::Load(const char* filename, const char* searchpath, const char* fieldname, bool doByteSwap)
{
    Clear();
    m_volume = BufferVolumeData::CreateInstance();

    std::vector<std::string> searchPaths;

    if (searchpath) {
        searchPaths.push_back(std::string(searchpath));
    } else {
        searchPaths.push_back("./");
    }

    std::string filepath = FindFile(searchPaths, std::string(filename));
    if (filepath.empty()) {
        fprintf(stderr, "[VTKLoader] File not found: %s\n", filename);
        return false;
    }

    lte::tinyvtkxml::TinyVTKXML toplevelXML(filepath.c_str());
    if (toplevelXML.Parse()) {
        fprintf(stderr, "[VTKLoader] Parsing failed. %s\n", filepath.c_str());
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
                    std::string source = FindFile(searchPaths, pieceArrays[i]->GetAttribute("Source"));
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
            // @todo { lod }
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
        //#ifdef OPENMP
        //#pragma omp parallel for shared(ok)
        //#endif
        for (size_t i = 0; i < m_pieceInfoList.size(); i++) {
            bool ret = LoadImageDataFile(m_volume->Buffer()->GetBuffer(), m_pieceInfoList[i].source, m_pieceInfoList[i], fieldname, doByteSwap, m_dataArrayInfoList[fieldIdx].numberOfComponents, globalDim);
            if (!ret) {
                ok = false; // Not using atomic op is probably OK for atomically updating bool value.
            }
        }

        return ok;
        
    } else if (type == "ImageData") { // ImageData VTK file
        printf("ImageDataNode found\n");
        // @todo
        assert(0);
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
