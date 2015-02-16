#include "BufferImageData.h"
#include "Buffer.h"
#include <vector>

class BufferImageData::Impl
{
private:
    int m_width;
    int m_height;
    FORMAT m_format;
    RefPtr<FloatBuffer> m_floatbuffer;
    RefPtr<ByteBuffer> m_bytebuffer;
    float m_gamma;
    float m_exposure;
    
public:
    Impl()
    {
        Clear();
    }
    Impl(BufferImageData* data)
    {
        Clear();
        m_width = data->Width();
        m_height = data->Height();
        m_format = data->Format();
        m_floatbuffer = data->FloatImageBuffer();
        m_bytebuffer = data->ImageBuffer();
        m_gamma = data->Gamma();
        m_exposure = data->Exposure();
    }
    
    void Create(FORMAT type, int w, int h)
    {
        m_width = w;
        m_height = h;
        m_format = type;
        if (type == RGB8) {
            m_bytebuffer = new ByteBuffer();
            m_bytebuffer->Create(w * h * 3);
        } else if (type == RGBA8) {
            m_bytebuffer = new ByteBuffer();
            m_bytebuffer->Create(w * h * 4);
        } else if (type == RGBA32F) {
            m_floatbuffer = new FloatBuffer();
            m_floatbuffer->Create(w * h * 4);
        }
    }
    
    void Clear()
    {
        m_width = 0;
        m_height = 0;
        m_format = INVALID;
        m_floatbuffer = NULL;
        m_bytebuffer = NULL;
        m_gamma = 1.0f;
        m_exposure = 1.0f;
    }
    
    int Width() const
    {
        return m_width;
    }
    
    int Height() const
    {
        return m_height;
    }
    
    FORMAT Format() const
    {
        return m_format;
    }
    
    int Bytes() const
    {
        int wxh = m_width * m_height;
        if (m_format == RGBA8) {
            return wxh * 4;
        } else if (m_format == RGB8) {
            return wxh * 3;
        } else if (m_format == RGBA32F) {
            return wxh * 4 * 4;
        }
        return -1;
    }

    FloatBuffer* FloatImageBuffer()
    {
        return m_floatbuffer;
    }
    
    FloatBuffer* FloatImageBuffer() const
    {
        return m_floatbuffer;
    }
    
    ByteBuffer* ImageBuffer()
    {
        return m_bytebuffer;
    }
    
    ByteBuffer* ImageBuffer() const
    {
        return m_bytebuffer;
    }
    
    float Gamma() const
    {
        return m_gamma;
    }
    
    float Exposure() const
    {
        return m_exposure;
    }
    
    void SetGamma(float gamma)
    {
        m_gamma = gamma;
    }
    
    void SetExposure(float exposure)
    {
        m_exposure = exposure;
    }
};

/// constructor
BufferImageData::BufferImageData()
{
    m_imp = new Impl();
}

/// constructor
BufferImageData::BufferImageData(BufferImageData* inst)
{
    m_imp = new Impl(inst);
}

/// destructor
BufferImageData::~BufferImageData()
{
    delete m_imp;
}

void BufferImageData::Create(FORMAT format, int w, int h)
{
    m_imp->Create(format, w, h);
}

void BufferImageData::Clear()
{
    m_imp->Clear();
}

int BufferImageData::Width() const
{
    return m_imp->Width();
}

int BufferImageData::Height() const
{
    return m_imp->Height();
}

BufferImageData::FORMAT BufferImageData::Format() const
{
    return m_imp->Format();
}

int BufferImageData::Bytes() const
{
    return m_imp->Bytes();
}

FloatBuffer* BufferImageData::FloatImageBuffer()
{
    return m_imp->FloatImageBuffer();
}

FloatBuffer* BufferImageData::FloatImageBuffer() const
{
    return m_imp->FloatImageBuffer();
}

ByteBuffer* BufferImageData::ImageBuffer()
{
    return m_imp->ImageBuffer();
}

ByteBuffer* BufferImageData::ImageBuffer() const
{
    return m_imp->ImageBuffer();
}

float BufferImageData::Gamma() const
{
    return m_imp->Gamma();
}

float BufferImageData::Exposure() const
{
    return m_imp->Exposure();
}

void BufferImageData::SetGamma(float gamma)
{
    m_imp->SetGamma(gamma);
}

void BufferImageData::SetExposure(float exposure)
{
    m_imp->SetExposure(exposure);
}

