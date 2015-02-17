#include "ImageLoader.h"
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
        std::transform(in.begin(), in.end(), std::back_inserter(out), ::tolower);
        return out;
    }
    
} // anonymouse namepsace

class ImageLoader::Impl
{
private:
    BufferImageData m_image;
    
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
    
    char* LoadFile(const std::string& filepath)
    {
        std::ifstream ifs(filepath.c_str(), std::ios::in | std::ios::binary);
        if (!ifs.good()) {
            printf("File Open Error");
            return false;
        }
        ifs.seekg(0, std::ifstream::end);
        const unsigned int fsize = ifs.tellg();
        ifs.seekg(0, std::ifstream::beg);
        char* buffer = new char[fsize];
        ifs.read(buffer, fsize);
        return buffer;
    }
    
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
    
    bool LoadHDR(const std::string& filepath)
    {
        int width = 0;
        int height = 0;
        const char* srcbuffer = LoadFile(filepath);
        float * dstbuffer = NULL;
        bool result = SimpleHDRLoader(filepath.c_str(), width, height, &dstbuffer);
        if (result && dstbuffer)
        {
            m_image.Create(BufferImageData::RGBA32F, width, height);
            memcpy(m_image.FloatImageBuffer()->GetBuffer(), dstbuffer, sizeof(float) * 4 * width * height);
        }
        delete [] srcbuffer;
        delete [] dstbuffer;
        return result;
    }
    
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
            }
            else if (ext == "jpg" || ext == "jpeg")
            {
                
            }
            else if (ext == "tga")
            {
                result = LoadTGA(path);
            }
            else if (ext == "hdr")
            {
                result = LoadHDR(path);
            }
        }
        return result;
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

