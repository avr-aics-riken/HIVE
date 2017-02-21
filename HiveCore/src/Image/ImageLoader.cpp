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
#include "ImageSaver.h"

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
    RefPtr<BufferImageData> m_image;
    
public:
    /// コンストラクタ
    Impl() {
        m_image = BufferImageData::CreateInstance();
        m_image->Clear();
    }

    /// デストラクタ
    ~Impl() {
        m_image->Clear();
    }
    
    /// イメージデータへの参照
    BufferImageData* ImageData()
    {
        return m_image;
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
     * TGAファイルをメモリからロード
     * @param srcbuffer TGAファイルバッファ 
     * @retval true 成功
     * @retval false 失敗
     */
    bool LoadTGAFromMemory(const void* srcbuffer)
    {
        int width = 0;
        int height = 0;
        unsigned char* dstbuffer = NULL;
        bool result = SimpleTGALoaderRGBA(srcbuffer, width, height, &dstbuffer);
        if (result && dstbuffer)
        {
            m_image = BufferImageData::CreateInstance();
            m_image->Create(BufferImageData::RGBA8, width, height);
            memcpy(m_image->ImageBuffer()->GetBuffer(), dstbuffer, sizeof(unsigned char) * 4 * width * height);
        }
        delete [] dstbuffer;
        return result;
    }

    /**
     * TGAファイルロード
     * @param filepath  ファイルフルパス 
     * @return result TGAロード結果
     */
    bool LoadTGA(const std::string& filepath)
    {
        const char* srcbuffer = LoadFile(filepath);
        bool result = LoadTGAFromMemory(reinterpret_cast<const void*>(srcbuffer));
        delete [] srcbuffer;
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
            m_image = BufferImageData::CreateInstance();
            m_image->Create(BufferImageData::RGBA32F, width, height);
            
            //memcpy(m_image.FloatImageBuffer()->GetBuffer(), dstbuffer, sizeof(float) * 4 * width * height);
            // flip y copy
            for (int y = 0; y < height; ++y) {
                const int src_offsety = sizeof(float) * width * y;
                const int dst_offsety = sizeof(float) * width * (height - 1 - y);
                memcpy(m_image->FloatImageBuffer()->GetBuffer() + src_offsety, dstbuffer + dst_offsety, sizeof(float) * 4 * width);
            }
        }
        delete [] dstbuffer;
        return result;
    }

    /**
     * PNGファイルをメモリからロード
     * @param srcbuffer PNGファイルバッファ
     * @param bufferSize PNGファイルバッファサイズ
     * @retval true 成功
     * @retval false 失敗
     */
    bool LoadPNGFromMemory(const void* srcbuffer, int bufferSize)
    {
        int width = 0;
        int height = 0;
        unsigned char * dstbuffer = NULL;
        const unsigned char* src = reinterpret_cast<const unsigned char*>(srcbuffer); 
        bool result = SimplePNGLoaderFromMemory(src, bufferSize, width, height, &dstbuffer);
        if (result && dstbuffer)
        {
            m_image = BufferImageData::CreateInstance();
            m_image->Create(BufferImageData::RGBA8, width, height);
            memcpy(m_image->ImageBuffer()->GetBuffer(), dstbuffer, sizeof(unsigned char) * 4 * width * height);
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
            m_image = BufferImageData::CreateInstance();
            m_image->Create(BufferImageData::RGBA8, width, height);
            memcpy(m_image->ImageBuffer()->GetBuffer(), dstbuffer, sizeof(unsigned char) * 4 * width * height);
        }
        delete [] dstbuffer;
        return result;
    }

    /**
     * JPGファイルをメモリからロード
     * @param srcbuffer JPGファイルバッファ
     * @param bufferSize JPGファイルバッファサイズ
     * @retval true 成功
     * @retval false 失敗
     */
    bool LoadJPGFromMemory(const void* srcbuffer, int bufferSize)
    {
        int width = 0;
        int height = 0;
        unsigned char * dstbuffer = NULL;
        const unsigned char * src = reinterpret_cast<const unsigned char*>(srcbuffer);
        bool result = SimpleJPGLoaderRGBAFromMemory(src, bufferSize, width, height, &dstbuffer);
        if (result && dstbuffer)
        {
            m_image = BufferImageData::CreateInstance();
            m_image->Create(BufferImageData::RGBA8, width, height);
            memcpy(m_image->ImageBuffer()->GetBuffer(), dstbuffer, sizeof(unsigned char) * 4 * width * height);
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
            m_image = BufferImageData::CreateInstance();
            m_image->Create(BufferImageData::RGBA8, width, height);
            memcpy(m_image->ImageBuffer()->GetBuffer(), dstbuffer, sizeof(unsigned char) * 4 * width * height);
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
            m_image = BufferImageData::CreateInstance();
            m_image->Create(BufferImageData::RGBA32F, width, height);
            memcpy(m_image->FloatImageBuffer()->GetBuffer(), dstbuffer, sizeof(float) * 4 * width * height);
        }
        delete [] dstbuffer;
        return result;
    }
    
    /**
     * RAWファイルロード
     * @param filepath  ファイルフルパス
     * @retval true 成功
     * @retval false 失敗
     */
    bool LoadRawFromPointer(int width, int height, int color, int bit, void* ptr)
    {
        printf("RAW: from pointer \n");
        printf(" %d x %d x %d x %dbit\n", width, height, color, bit);
        if (color !=4 || bit != 8) { // temp
            return false;
        }
        
        m_image = BufferImageData::CreateInstance();
        m_image->Create(BufferImageData::RGBA8, width, height);
        memcpy(m_image->ImageBuffer()->GetBuffer(), ptr, sizeof(unsigned char) * 4 * width * height);

        return true;
    }
    
    /**
     * BufferImageDataから画像をロード(DeepCopy)
     * @param BufferImageData* 画像データ
     * @retval true 成功
     * @retval false 失敗
     */
    bool LoadImageData(BufferImageData* imageData)
    {
        const int width = imageData->Width();
        const int height = imageData->Height();
        m_image = BufferImageData::CreateInstance();
        m_image->Create(imageData->Format(), width, height);
        
        if (imageData->Format() == BufferImageData::RGB8 ||
            imageData->Format() == BufferImageData::RGBA8)
        {
            memcpy(m_image->ImageBuffer()->GetBuffer(), 
                    imageData->ImageBuffer()->GetBuffer(),
                    imageData->ImageBuffer()->GetNum());
            return true;
        }
        else if (imageData->Format() == BufferImageData::R32F ||
                 imageData->Format() == BufferImageData::RGBA32F)
        {
            memcpy(m_image->FloatImageBuffer()->GetBuffer(), 
                    imageData->FloatImageBuffer()->GetBuffer(),
                    imageData->FloatImageBuffer()->GetNum());
            return true;
        }
        
        return false;
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
        m_image  = BufferImageData::CreateInstance();
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
     * メモリから画像ファイルをロード
     * @param format 画像フォーマット
     * @param data 画像バイナリポインタ
     */
    bool LoadMemory(unsigned int format, const Buffer buffer, unsigned int bufferSize)
    {
        m_image  = BufferImageData::CreateInstance();
        if (format == ImageSaver::TGA) {
            return LoadTGAFromMemory(buffer);
        }
        else if (format == ImageSaver::JPG) {
            return LoadJPGFromMemory(buffer, bufferSize);
        }
        else if (format == ImageSaver::PNG) {
            return LoadPNGFromMemory(buffer, bufferSize);
        }
        else
        {
            // NOT IMPLEMENTED
        }
        return false;
    }

    /**
     * ロードしたファイルイメージバッファ
     * @retval Buffer ファイルイメージバッファ
     */
    ImageLoader::Buffer ImageBuffer() const
    {
        unsigned char* buf = m_image->ImageBuffer()->GetBuffer();
        return reinterpret_cast<ImageLoader::Buffer>(buf);
    }

    /**
     * ロードしたファイルイメージバッファサイズ
     * @retval GetNum イメージバッファサイズ
     */
    int ImageBufferSize() const
    {
        return m_image->ImageBuffer()->GetNum();
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

bool ImageLoader::LoadMemory(unsigned int format, const Buffer buffer, unsigned int bufferSize)
{
    return m_imp->LoadMemory(format, buffer, bufferSize);
}

BufferImageData* ImageLoader::ImageData()
{
    return m_imp->ImageData();
}

/// @retval no compressed buffer
ImageLoader::Buffer ImageLoader::ImageBuffer() const
{
    return m_imp->ImageBuffer();
}

/// @retval no compressed buffer size
int ImageLoader::ImageBufferSize() const
{
    return m_imp->ImageBufferSize();
}

bool ImageLoader::LoadRawFromPointer(int width, int height, int color, int bit, void* ptr)
{
    return m_imp->LoadRawFromPointer(width, height, color, bit, ptr);
}

bool ImageLoader::LoadImageData(BufferImageData* imageData)
{
    return m_imp->LoadImageData(imageData);
}
