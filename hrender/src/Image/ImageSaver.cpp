/**
 * @file ImageSaver.cpp
 * イメージファイルセーブユーティリティ
 */
#include "ImageSaver.h"
#include <stdio.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "BufferImageData.h"
#include "../Image/SimpleJPG.h"
#include "../Image/SimplePNG.h"
#include "../Image/SimpleTGA.h"
#include "../Image/SimpleEXR.h"
#include "../Image/SimpleHDR.h"
#include "Buffer.h"

namespace
{
    std::string make_lowercase(const std::string& in)
    {
        std::string out;
        std::transform( in.begin(), in.end(), std::back_inserter( out ), ::tolower );
        return out;
    }
} // anonymouse namepsace

/**
 * 画像セーバ
 */
class ImageSaver::Impl
{
private:
    BufferImageData m_image;
    std::string m_memory;
    
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
     * @retval true 成功
     * @retval false 失敗
     */
    bool SaveFile(const std::string& filepath, const unsigned char* data, int bytes)
    {
        std::ofstream ofs(filepath.c_str(), std::ios::out | std::ios::binary);
        if (!ofs.good()) {
            fprintf(stderr,"File Open Error");
            return false;
        }
        ofs.write(reinterpret_cast<const char*>(data), bytes);
        return true;
    }

    /**
     * ファイルロード
     * @param filepath  ファイルフルパス 
     * @param data      ファイルデータ
     * @retval true 成功
     * @retval false 失敗
     */
    bool Save(const char* filename, BufferImageData* data)
    {
        if (!filename) { return false; }
        if (!data) { return false; }
        bool result = false;
        
        std::string path(filename);
        std::string::size_type pos = path.rfind('.');
        std::string ext;
        if (pos != std::string::npos)
        {
            ext = make_lowercase(path.substr(pos+1));
        }
        else
        {
            ext = "jpg";
            path += ".jpg";
        }
        
        const int width = data->Width();
        const int height = data->Height();
        if (ext == "png" || ext == "PNG")
        {
            const unsigned char* srcbuffer = data->ImageBuffer()->GetBuffer();
            if (data->Format() == BufferImageData::RGBA8)
            {
                unsigned char* pngbuffer = NULL;
                int bytes = SimplePNGSaverRGBA((void**)&pngbuffer, width, height, srcbuffer);
                //printf("PNG: bytes: %d\n", bytes);
                if (bytes && pngbuffer)
                {
                    result = SaveFile(path, pngbuffer, bytes);
                }
                delete [] pngbuffer;
            }
        }
        else if (ext == "jpg" || ext == "jpeg")
        {
            const unsigned char* srcbuffer = data->ImageBuffer()->GetBuffer();
            if (data->Format() == BufferImageData::RGBA8)
            {
                unsigned char* jpgbuffer = NULL;
                int bytes = SimpleJPGSaverRGBA((void**)&jpgbuffer, width, height, srcbuffer);
                if (bytes && jpgbuffer)
                {
                    result = SaveFile(path, jpgbuffer, bytes);
                }
                delete [] jpgbuffer;
            }
        }
        else if (ext == "tga")
        {
            const unsigned char* srcbuffer = data->ImageBuffer()->GetBuffer();
            if (data->Format() == BufferImageData::RGBA8)
            {
                unsigned char* tgabuffer = NULL;
                const int bytes = SimpleTGASaverRGBA((void**)&tgabuffer, width, height, srcbuffer);
                if (bytes && tgabuffer)
                {
                    result = SaveFile(path, tgabuffer, bytes);
                }
                delete [] tgabuffer;
            }
        }
        else if (ext == "exr" || ext == "EXR")
        {
            if (data->FloatImageBuffer()) { // rendererd onto HDR buffer
                const float* srcbuffer = data->FloatImageBuffer()->GetBuffer();
                if (data->Format() == BufferImageData::RGBA32F)
                {
                    unsigned char* exrbuffer = NULL;
                    const int bytes = SimpleEXRSaverRGBA((void**)&exrbuffer, width, height, srcbuffer);
                    if (bytes && exrbuffer)
                    {
                        result = SaveFile(path, exrbuffer, bytes);
                    }
                    delete [] exrbuffer;
                }
                else if (data->Format() == BufferImageData::R32F)
                {
                    unsigned char* exrbuffer = NULL;
                    const int bytes = SimpleEXRSaverZ((void**)&exrbuffer, width, height, srcbuffer);
                    if (bytes && exrbuffer)
                    {
                        result = SaveFile(path, exrbuffer, bytes);
                    }
                    delete [] exrbuffer;
                }
            } else {
                const unsigned char* srcbuffer = data->ImageBuffer()->GetBuffer();
                if (data->Format() == BufferImageData::RGBA8) {
                    // BYTE -> float
                    float *hdrbuffer = new float[width*height*4];
                    for (size_t i = 0; i < width * height * 4; i++) {
                        hdrbuffer[i] = (float)srcbuffer[i] / 255.5f;
                    }

                    unsigned char* exrbuffer = NULL;
                    const int bytes = SimpleEXRSaverRGBA((void**)&exrbuffer, width, height, hdrbuffer);
                    if (bytes && exrbuffer)
                    {
                        result = SaveFile(path, exrbuffer, bytes);
                    }
                    delete [] exrbuffer;
                    delete [] hdrbuffer;
                }
            }
        }
        else if (ext == "hdr")
        {
            if (data->FloatImageBuffer()) { // rendererd onto HDR buffer
                const float* srcbuffer = data->FloatImageBuffer()->GetBuffer();
                if (data->Format() == BufferImageData::RGBA32F)
                {
                    result = SimpleHDRSaver(path.c_str(), width, height, srcbuffer);
                }
                else if (data->Format() == BufferImageData::R32F)
                {
                    // TODO:
                    assert(0);
                    printf("TODO: implementation. R32F HDR saver\n");
                }
            } else {
                const unsigned char* srcbuffer = data->ImageBuffer()->GetBuffer();
                if (data->Format() == BufferImageData::RGBA8) {
                    // BYTE -> float
                    float *hdrbuffer = new float[width*height*4];
                    for (size_t i = 0; i < width * height * 4; i++) {
                        hdrbuffer[i] = (float)srcbuffer[i] / 255.5f;
                    }
                    
                    result = SimpleHDRSaver(path.c_str(), width, height, hdrbuffer);
                    
                    delete [] hdrbuffer;
                }
            }
        }
    
        if (result) {
            printf("Save:%s\n", path.c_str());
        }
        return result;
    }

    /**
     * メモリロード
    * @param format  ファイルフォーマット(JPG, TGAのみ)
     * @param data      ファイルデータ
     * @return buffer ファイルイメージデータバッファ
     */
    const Buffer SaveMemory(unsigned int format, BufferImageData* data)
    {
        if (!data) { return NULL; }
        if (data->Bytes() <= 0) { return NULL; }
        const int width = data->Width();
        const int height = data->Height();
        char* dstbuffer = NULL;
        
        if (format == ImageSaver::JPG)
        {
            const unsigned char* srcbuffer = data->ImageBuffer()->GetBuffer();
            const int bytes = SimpleJPGSaverRGBA((void**)&dstbuffer, width, height, srcbuffer);
            if (bytes && dstbuffer) {
                m_memory = std::string(dstbuffer, bytes);
                delete [] dstbuffer;
                return (const Buffer)m_memory.c_str();
            }
        }
        else if (format == ImageSaver::TGA)
        {
            const unsigned char* srcbuffer = data->ImageBuffer()->GetBuffer();
            const int bytes = SimpleTGASaverRGBA((void**)&dstbuffer, width, height, srcbuffer);
            if (bytes && dstbuffer) {
                m_memory = std::string(dstbuffer, bytes);
                delete [] dstbuffer;
                return (const Buffer)m_memory.c_str();
            }
        }
        else if (format == ImageSaver::EXR)
        {
            const float* srcbuffer = data->FloatImageBuffer()->GetBuffer();
            if (data->Format() == BufferImageData::R32F) {
                const int bytes = SimpleEXRSaverZ((void**)&dstbuffer, width, height, srcbuffer);
                if (bytes && dstbuffer) {
                    m_memory = std::string(dstbuffer, bytes);
                    delete [] dstbuffer;
                    return (const Buffer)m_memory.c_str();
                }
            } else if (data->Format() == BufferImageData::RGBA32F) {
                const int bytes = SimpleEXRSaverRGBA((void**)&dstbuffer, width, height, srcbuffer);
                if (bytes && dstbuffer) {
                    m_memory = std::string(dstbuffer, bytes);
                    delete [] dstbuffer;
                    return (const Buffer)m_memory.c_str();
                }
            }
        }
        else if (format == ImageSaver::HDR)
        {
            // not implemented
        }
        return NULL;
    }
    
    int MemorySize() const
    {
        return static_cast<int>(m_memory.size());
    }
};

/// constructor
ImageSaver::ImageSaver()
{
    m_imp = new ImageSaver::Impl();
}

/// destructor
ImageSaver::~ImageSaver()
{
    delete m_imp;
}

bool ImageSaver::Save(const char* filename, BufferImageData* data)
{
    return m_imp->Save(filename, data);
}

const ImageSaver::Buffer ImageSaver::SaveMemory(unsigned int format, BufferImageData* data)
{
    return m_imp->SaveMemory(format, data);
}

int ImageSaver::MemorySize() const
{
    return m_imp->MemorySize();
}
