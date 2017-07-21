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
    
    unsigned char toByte(float f)
    {
        return static_cast<unsigned char>((std::min)(f * 0xFF, float(0xFF)) + 0.5);
    }
    
    float toFloat(char b)
    {
        return static_cast<float>(b / 255.0f);
    }
    
    
    void convertRGBA32FToRGBA8Buffer(unsigned char* byteBuffer, const float* floatBuffer, const int length)
    {
        for (int i = 0; i < length; ++i) {
            byteBuffer[i] = toByte(floatBuffer[i]);
        }
    }
        
    BufferImageData* convertRGBA32FToRGBA8(BufferImageData* data)
    {
        const float* src = data->FloatImageBuffer()->GetBuffer();
        const int size = data->FloatImageBuffer()->GetNum();
        BufferImageData* dst = BufferImageData::CreateInstance();
        dst->Create(BufferImageData::RGBA8, data->Width(), data->Height());
        unsigned char* dstBuffer = dst->ImageBuffer()->GetBuffer();
        convertRGBA32FToRGBA8Buffer(dstBuffer, src, size);
        return dst;
    }
    
    void convertR32FToRGBA8Buffer(unsigned char* byteBuffer, const float* floatBuffer, const int length)
    {
        for (int i = 0; i < length; ++i) {
            byteBuffer[i * 4 + 0] = toByte(floatBuffer[i]);
            byteBuffer[i * 4 + 1] = byteBuffer[i * 4 + 0];
            byteBuffer[i * 4 + 2] = byteBuffer[i * 4 + 0];
            byteBuffer[i * 4 + 3] = 0xFF;
        }
    }
        
    BufferImageData* convertR32FToRGBA8(BufferImageData* data)
    {
        const float* src = data->FloatImageBuffer()->GetBuffer();
        const int size = data->FloatImageBuffer()->GetNum();
        BufferImageData* dst = BufferImageData::CreateInstance();
        dst->Create(BufferImageData::RGBA8, data->Width(), data->Height());
        unsigned char* dstBuffer = dst->ImageBuffer()->GetBuffer();
        convertR32FToRGBA8Buffer(dstBuffer, src, size);
        return dst;
    }
    
    void convertRGBA8ToRGBA32FBuffer(float* floatBuffer, const unsigned char* byteBuffer, const int length)
    {
        for (int i = 0; i < length; ++i) {
            floatBuffer[i] = toFloat(byteBuffer[i]);
        }
    }
    
    void convertRGB8ToRGBA32FBuffer(float* floatBuffer, const unsigned char* byteBuffer, const int length)
    {
        int size = length / 3;
        for (int i = 0; i < size; ++i) {
            floatBuffer[i * 4 + 0] = toFloat(byteBuffer[i * 3 + 0]);
            floatBuffer[i * 4 + 1] = toFloat(byteBuffer[i * 3 + 1]);
            floatBuffer[i * 4 + 2] = toFloat(byteBuffer[i * 3 + 2]);
            floatBuffer[i * 4 + 3] = 1.0f;
        }
    }
    
    BufferImageData* convertRGBA8ToRGBA32F(BufferImageData* data)
    {
        const unsigned char* src = data->ImageBuffer()->GetBuffer();
        const int size = data->ImageBuffer()->GetNum();
        BufferImageData* dst = BufferImageData::CreateInstance();
        dst->Create(BufferImageData::RGBA32F, data->Width(), data->Height());
        float* dstBuffer = dst->FloatImageBuffer()->GetBuffer();
        convertRGBA8ToRGBA32FBuffer(dstBuffer, src, size);
        return dst;
    }
    
    BufferImageData* convertRGB8FToRGBA32F(BufferImageData* data)
    {
        const unsigned char* src = data->ImageBuffer()->GetBuffer();
        const int size = data->ImageBuffer()->GetNum();
        BufferImageData* dst = BufferImageData::CreateInstance();
        dst->Create(BufferImageData::RGBA32F, data->Width(), data->Height());
        float* dstBuffer = dst->FloatImageBuffer()->GetBuffer();
        convertRGB8ToRGBA32FBuffer(dstBuffer, src, size);
        return dst;
    }
    
} // anonymouse namepsace

/**
 * 画像セーバ
 */
class ImageSaver::Impl
{
private:
    std::string m_memory;
    
public:
    /// コンストラクタ
    Impl() {}

    /// デストラクタ
    ~Impl() {}

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
        if (std::string(filename) == "") { return false; }
        const int width = data->Width();
        const int height = data->Height();
        
        if (width == 0 || height == 0) { return false; }
        
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
        
        int format = 1; // default jpg
        if (ext == "png") {
            format = ImageSaver::PNG;
        } else if (ext == "tga") {
            format = ImageSaver::TGA;
        } else if (ext == "exr") {
            format = ImageSaver::EXR;
        } else if (ext == "hdr") {
            format = ImageSaver::HDR;
        }
        
        // convert float <==> byte buffers
        if (format == ImageSaver::JPG || format == ImageSaver::TGA || format == ImageSaver::PNG)
        {
            if (data->Format() == BufferImageData::RGBA32F) {
                data = convertRGBA32FToRGBA8(data);
            }
            if (data->Format() == BufferImageData::R32F) {
                data = convertR32FToRGBA8(data);
            }
        }
        
        if (format == ImageSaver::PNG)
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
        else if (format == ImageSaver::JPG)
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
                free(jpgbuffer);
            }
        }
        else if (format == ImageSaver::TGA)
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
                free(tgabuffer);
            }
        }
        else if (format == ImageSaver::EXR)
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
        else if (format == ImageSaver::HDR)
        {
            if (data->FloatImageBuffer()) { // rendererd onto HDR buffer
                if (data->Format() == BufferImageData::RGBA32F)
                {
                    // flip y copy
                    float *hdrbuffer = new float[width*height*4];
                    for (int y = 0; y < height; ++y) {
                        const int src_offsety = sizeof(float) * width * y;
                        const int dst_offsety = sizeof(float) * width * (height - 1 - y);
                        memcpy(hdrbuffer + dst_offsety, data->FloatImageBuffer()->GetBuffer() + src_offsety, sizeof(float) * 4 * width);
                    }

                    result = SimpleHDRSaver(path.c_str(), width, height, hdrbuffer);
                    delete [] hdrbuffer;
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
                    // flip y copy
                    float *hdrbuffer = new float[width*height*4];
                    for (size_t y = 0; y < height; y++) {
                        for (size_t x = 0; x < width; x++) {
                            const int src_offset_i = 4 * (width * y + x);
                            const int dst_offset_i = 4 * (width * (height - 1 - y) + x);
                            hdrbuffer[dst_offset_i    ] = (float)srcbuffer[src_offset_i    ] / 255.5f;
                            hdrbuffer[dst_offset_i + 1] = (float)srcbuffer[src_offset_i + 1] / 255.5f;
                            hdrbuffer[dst_offset_i + 2] = (float)srcbuffer[src_offset_i + 2] / 255.5f;
                            hdrbuffer[dst_offset_i + 3] = (float)srcbuffer[src_offset_i + 3] / 255.5f;
                        }
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
    * @param format  ファイルフォーマット(JPG, TGA, EXR, PNG)
     * @param data      ファイルデータ
     * @return buffer ファイルイメージデータバッファ
     */
    Buffer SaveMemory(unsigned int format, BufferImageData* data)
    {
        if (!data) { return NULL; }
        if (data->Bytes() <= 0) { return NULL; }
        const int width = data->Width();
        const int height = data->Height();
        char* dstbuffer = NULL;
        
        // convert float <==> byte buffers
        if (format == ImageSaver::JPG || format == ImageSaver::TGA || format == ImageSaver::PNG)
        {
            if (data->Format() == BufferImageData::RGBA32F) {
                data = convertRGBA32FToRGBA8(data);
            }
            if (data->Format() == BufferImageData::R32F) {
                data = convertR32FToRGBA8(data);
            }
        } else if (format == ImageSaver::EXR) {
            if (data->Format() == BufferImageData::RGBA8) {
                data = convertRGBA8ToRGBA32F(data);
            }
            if (data->Format() == BufferImageData::RGB8) {
                data = convertRGB8FToRGBA32F(data);
            }
        }
        
        // save image to memory
        if (format == ImageSaver::JPG)
        {
            const unsigned char* srcbuffer = data->ImageBuffer()->GetBuffer();
            const int bytes = SimpleJPGSaverRGBA((void**)&dstbuffer, width, height, srcbuffer);
            if (bytes && dstbuffer) {
                m_memory = std::string(dstbuffer, bytes);
                free(dstbuffer); // Assume malloc is used in `jpge.cpp`
                return (Buffer)m_memory.c_str();
            }
        }
        else if (format == ImageSaver::TGA)
        {
            const unsigned char* srcbuffer = data->ImageBuffer()->GetBuffer();
            const int bytes = SimpleTGASaverRGBA((void**)&dstbuffer, width, height, srcbuffer);
            if (bytes && dstbuffer) {
                m_memory = std::string(dstbuffer, bytes);
                free(dstbuffer);
                return (Buffer)m_memory.c_str();
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
                    return (Buffer)m_memory.c_str();
                }
            } else if (data->Format() == BufferImageData::RGBA32F) {
                const int bytes = SimpleEXRSaverRGBA((void**)&dstbuffer, width, height, srcbuffer);
                if (bytes && dstbuffer) {
                    m_memory = std::string(dstbuffer, bytes);
                    delete [] dstbuffer;
                    return (Buffer)m_memory.c_str();
                }
            }
        }
        else if (format == ImageSaver::HDR)
        {
            // not implemented
        }
        else if (format == ImageSaver::PNG)
        {
            const unsigned char* srcbuffer = data->ImageBuffer()->GetBuffer();
            const int bytes = SimplePNGSaverRGBA((void**)&dstbuffer, width, height, srcbuffer);
            if (bytes && dstbuffer) {
                m_memory = std::string(dstbuffer, bytes);
                delete [] dstbuffer;
                return (Buffer)m_memory.c_str();
            }
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

ImageSaver::Buffer ImageSaver::SaveMemory(unsigned int format, BufferImageData* data)
{
    return m_imp->SaveMemory(format, data);
}

int ImageSaver::MemorySize() const
{
    return m_imp->MemorySize();
}
