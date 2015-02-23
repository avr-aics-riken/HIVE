#include "ImageSaver.h"
#include <stdio.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "BufferImageData.h"
#include "../Image/SimpleJPG.h"
#include "../Image/SimpleTGA.h"
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

class ImageSaver::Impl
{
private:
    BufferImageData m_image;
    std::string m_memory;
    
public:
    Impl() {
        m_image.Clear();
    }
    ~Impl() {
        m_image.Clear();
    }
    
    BufferImageData* ImageData()
    {
        return &m_image;
    }
    
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
    
    bool Save(const char* filename, BufferImageData* data)
    {
        if (!filename) { return false; }
        if (!data) { return false; }
        bool result = false;
        
        std::string path(filename);
        std::string::size_type pos = path.rfind('.');
        if (pos != std::string::npos)
        {
            const std::string ext = make_lowercase(path.substr(pos+1));
            const int width = data->Width();
            const int height = data->Height();
            if (ext == "png")
            {
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
            else if (ext == "hdr")
            {
            }
        }
        if (result) {
            printf("Save:%s\n", filename);
        }
        return result;
    }
    
    const Buffer SaveMemory(unsigned int format, BufferImageData* data)
    {
        if (!data) { return NULL; }
        if (data->Bytes() <= 0) { return NULL; }
        const unsigned char* srcbuffer = data->ImageBuffer()->GetBuffer();
        const int width = data->Width();
        const int height = data->Height();
        char* dstbuffer = NULL;
        
        if (format == ImageSaver::JPG)
        {
            const int bytes = SimpleJPGSaverRGBA((void**)&dstbuffer, width, height, srcbuffer);
            if (bytes && dstbuffer) {
                m_memory = std::string(dstbuffer, bytes);
                //delete [] dstbuffer;
                return (const Buffer)m_memory.c_str();
            }
        }
        else if (format == ImageSaver::TGA)
        {
            const int bytes = SimpleTGASaverRGBA((void**)&dstbuffer, width, height, srcbuffer);
            if (bytes && dstbuffer) {
                m_memory = std::string(dstbuffer, bytes);
                //delete [] dstbuffer;
                return (const Buffer)m_memory.c_str();
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
