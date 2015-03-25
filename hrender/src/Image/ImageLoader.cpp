/**
 * @file ImageLoader.cpp
 * イメージローダー
 */
#include "ImageLoader.h"
#include <stdio.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "BufferImageData.h"
#include "../Image/SimpleJPG.h"
#include "../Image/SimpleTGA.h"
#include "../Image/SimplePNG.h"
#include "../Image/SimpleHDR.h"
#include "../Image/SimpleEXR.h"
#include "Buffer.h"

namespace
{
    std::string make_lowercase(const std::string& in)
    {
        std::string out;
        std::transform(in.begin(), in.end(), std::back_inserter(out), ::tolower);
        return out;
    }
    
} // anonymouse namepsace

/**
 * イメージローダー
 */
class ImageLoader::Impl
{
private:
    BufferImageData m_image;
    
public:
    /// コンストラクタ
    Impl() {
        m_image.Clear();
    }

    /// デストラクタ
    ~Impl() {
        m_image.Clear();
    }
    
    /// イメージデータへの参照
    BufferImageData* ImageData()
    {
        return &m_image;
    }

    /**
     * ファイルロード
     * @param filepath  ファイルフルパス 
     * @retval buffer Readしたファイルバッファ
     */
    char* LoadFile(const std::string& filepath)
    {
        std::ifstream ifs(filepath.c_str(), std::ios::in | std::ios::binary);
        if (!ifs.good()) {
            fprintf(stderr,"File Open Error");
            return NULL;
        }
        ifs.seekg(0, std::ifstream::end);
        const unsigned int fsize = ifs.tellg();
        ifs.seekg(0, std::ifstream::beg);
        char* buffer = new char[fsize];
        ifs.read(buffer, fsize);
        return buffer;
    }

    /**
     * TGAファイルロード
     * @param filepath  ファイルフルパス 
     * @return result TGAロード結果
     */
    bool LoadTGA(const std::string& filepath)
    {
        int width = 0;
        int height = 0;
        const char* srcbuffer = LoadFile(filepath);
        unsigned char* dstbuffer = NULL;
        bool result = SimpleTGALoaderRGBA(srcbuffer, width, height, &dstbuffer);
        if (result && dstbuffer)
        {
            m_image.Create(BufferImageData::RGBA8, width, height);
            memcpy(m_image.ImageBuffer()->GetBuffer(), dstbuffer, sizeof(unsigned char) * 4 * width * height);
        }
        delete [] srcbuffer;
        delete [] dstbuffer;
        return result;
    }

    /**
     * HDRファイルロード
     * @param filepath  ファイルフルパス 
     * @retval true 成功
     * @retval false 失敗
     */
    bool LoadHDR(const std::string& filepath)
    {
        int width = 0;
        int height = 0;
        float * dstbuffer = NULL;
        bool result = SimpleHDRLoader(filepath.c_str(), width, height, &dstbuffer);
        if (result && dstbuffer)
        {
            m_image.Create(BufferImageData::RGBA32F, width, height);
            
            //memcpy(m_image.FloatImageBuffer()->GetBuffer(), dstbuffer, sizeof(float) * 4 * width * height);
            // flip y copy
            for (int y = 0; y < height; ++y) {
                const int src_offsety = sizeof(float) * width * y;
                const int dst_offsety = sizeof(float) * width * (height - 1 - y);
                memcpy(m_image.FloatImageBuffer()->GetBuffer() + src_offsety, dstbuffer + dst_offsety, sizeof(float) * 4 * width);
            }
        }
        delete [] dstbuffer;
        return result;
    }

    /**
     * PNGファイルロード
     * @param filepath  ファイルフルパス 
     * @retval true 成功
     * @retval false 失敗
     */
    bool LoadPNG(const std::string& filepath)
    {
        int width = 0;
        int height = 0;
        unsigned char * dstbuffer = NULL;
        bool result = SimplePNGLoader(filepath.c_str(), width, height, &dstbuffer);
        if (result && dstbuffer)
        {
            m_image.Create(BufferImageData::RGBA8, width, height);
            memcpy(m_image.ImageBuffer()->GetBuffer(), dstbuffer, sizeof(unsigned char) * 4 * width * height);
        }
        delete [] dstbuffer;
        return result;
    }

    /**
     * JPGファイルロード
     * @param filepath  ファイルフルパス 
     * @retval true 成功
     * @retval false 失敗
     */
    bool LoadJPG(const std::string& filepath)
    {
        int width = 0;
        int height = 0;
        unsigned char * dstbuffer = NULL;
        bool result = SimpleJPGLoaderRGBA(filepath.c_str(), width, height, &dstbuffer);
        if (result && dstbuffer)
        {
            m_image.Create(BufferImageData::RGBA8, width, height);
            memcpy(m_image.ImageBuffer()->GetBuffer(), dstbuffer, sizeof(unsigned char) * 4 * width * height);
        }
        delete [] dstbuffer;
        return result;
    }

    /**
     * EXRファイルロード
     * @param filepath  ファイルフルパス 
     * @retval true 成功
     * @retval false 失敗
     */
    bool LoadEXR(const std::string& filepath)
    {
        int width = 0;
        int height = 0;
        float * dstbuffer = NULL;
        printf("EXR: \n");
        bool result = SimpleEXRLoaderRGBA(filepath.c_str(), width, height, &dstbuffer);
        printf("ret: %d: %d x %d \n", result, width, height);
        if (result && dstbuffer)
        {
            m_image.Create(BufferImageData::RGBA32F, width, height);
            memcpy(m_image.FloatImageBuffer()->GetBuffer(), dstbuffer, sizeof(float) * 4 * width * height);
        }
        delete [] dstbuffer;
        return result;
    }

    /**
     * ファイルロード[拡張子自動判別]
     * @param filepath  ファイルフルパス 
     * @retval true 成功
     * @retval false 失敗
     */
    bool Load(const char* filename)
    {
        bool result = false;
        m_image.Clear();
        std::string path(filename);
        std::string::size_type pos = path.rfind('.');
        if (pos != std::string::npos)
        {
            const std::string ext = make_lowercase(path.substr(pos+1));
            if (ext == "png")
            {
                result = LoadPNG(path);
            }
            else if (ext == "jpg" || ext == "jpeg")
            {
                result = LoadJPG(path);
            }
            else if (ext == "tga")
            {
                result = LoadTGA(path);
            }
            else if (ext == "hdr")
            {
                result = LoadHDR(path);
            }
            else if (ext == "exr" || ext == "EXR")
            {
                result = LoadEXR(path);
            }
        }
        return result;
    }

    /**
     * ロードしたファイルイメージバッファ
     * @retval Buffer ファイルイメージバッファ
     */
    const ImageLoader::Buffer ImageBuffer() const
    {
        unsigned char* buf = m_image.ImageBuffer()->GetBuffer();
        return reinterpret_cast<ImageLoader::Buffer>(buf);
    }

    /**
     * ロードしたファイルイメージバッファサイズ
     * @retval GetNum イメージバッファサイズ
     */
    int ImageBufferSize() const
    {
        return m_image.ImageBuffer()->GetNum();
    }
};

/// constructor
ImageLoader::ImageLoader()
{
    m_imp = new ImageLoader::Impl();
}

/// destructor
ImageLoader::~ImageLoader()
{
    delete m_imp;
}

bool ImageLoader::Load(const char* filename)
{
    return m_imp->Load(filename);
}

BufferImageData* ImageLoader::ImageData()
{
    return m_imp->ImageData();
}

/// @retval no compressed buffer
const ImageLoader::Buffer ImageLoader::ImageBuffer() const
{
    return m_imp->ImageBuffer();
}

/// @retval no compressed buffer size
int ImageLoader::ImageBufferSize() const
{
    return m_imp->ImageBufferSize();
}
