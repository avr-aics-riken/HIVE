#include "BufferVolumeData.h"
#include "Buffer.h"
#include <vector>

class BufferVolumeData::Impl
{
private:
    int m_dim[3];
    int m_comp;
    bool m_isNonUniform;
    RefPtr<FloatBuffer>  m_buffer;
    RefPtr<FloatBuffer>  m_spacingX;
    RefPtr<FloatBuffer>  m_spacingY;
    RefPtr<FloatBuffer>  m_spacingZ;
    
public:
    
    Impl()
    {
        Clear();
    }
    
    ~Impl()
    {
        Clear();
    }
    
    Impl(BufferVolumeData* inst)
    {
        this->m_dim[0] = inst->Width();
        this->m_dim[1] = inst->Height();
        this->m_dim[2] = inst->Depth();
        this->m_comp   = inst->Component();
        this->m_buffer = inst->Buffer();
        this->m_isNonUniform = inst->NonUniform();
        this->m_spacingX = inst->SpacingX();
        this->m_spacingY = inst->SpacingY();
        this->m_spacingZ = inst->SpacingZ();
    }
    
    void Create(int w, int h, int d, int component)
    {
        this->m_dim[0] = w;
        this->m_dim[1] = h;
        this->m_dim[2] = d;
        this->m_comp = component;
        FloatBuffer* buf = new FloatBuffer();
        FloatBuffer* spacingX = new FloatBuffer();
        FloatBuffer* spacingY = new FloatBuffer();
        FloatBuffer* spacingZ = new FloatBuffer();
        buf->Create(w * h * d * component);
        this->m_buffer = buf;
        spacingX->Create(w);
        spacingY->Create(h);
        spacingZ->Create(d);
        this->m_spacingX = spacingX;
        this->m_spacingY = spacingY;
        this->m_spacingZ = spacingZ;
        this->m_isNonUniform = false;
    }
    
    void Clear()
    {
        m_dim[0] = m_dim[1] = m_dim[2] = 0;
        m_comp   = 0;
        m_buffer = 0;
        m_isNonUniform = false;
    }
    
    void print()
    {
    }
    
    int Width() {
        return m_dim[0];
    }
    
    int Height() {
        return m_dim[1];
    }
    
    int Depth() {
        return m_dim[2];
    }
    
    int Component() {
        return m_comp;
    }
    
    FloatBuffer *Buffer() {
        return m_buffer;
    }
    
    bool& NonUniform() {
        return m_isNonUniform;
    }
    
    FloatBuffer* SpacingX() {
        return m_spacingX;
    }
    
    FloatBuffer* SpacingY() {
        return m_spacingY;
    }
    
    FloatBuffer* SpacingZ() {
        return m_spacingZ;
    }
};

/// constructor
BufferVolumeData::BufferVolumeData()
{
    m_imp = new BufferVolumeData::Impl();
}

/// constructor
BufferVolumeData::BufferVolumeData(BufferVolumeData* inst)
{
    m_imp = new BufferVolumeData::Impl(inst);
}

/// destructor
BufferVolumeData::~BufferVolumeData()
{
    delete m_imp;
}

void BufferVolumeData::Create(int w, int h, int d, int component)
{
    m_imp->Create(w, h, d, component);
}

void BufferVolumeData::Clear()
{
    m_imp->Clear();
}

void BufferVolumeData::print()
{
    m_imp->print();
}

int BufferVolumeData::Width() {
    return m_imp->Width();
}

int BufferVolumeData::Height() {
    return m_imp->Height();
}

int BufferVolumeData::Depth() {
    return m_imp->Depth();
}

int BufferVolumeData::Component() {
    return m_imp->Depth();
}

FloatBuffer *BufferVolumeData::Buffer() {
    return m_imp->Buffer();
}

bool& BufferVolumeData::NonUniform() {
    return m_imp->NonUniform();
}

FloatBuffer* BufferVolumeData::SpacingX() {
    return m_imp->SpacingX();
}

FloatBuffer* BufferVolumeData::SpacingY() {
    return m_imp->SpacingY();
}

FloatBuffer* BufferVolumeData::SpacingZ() {
    return m_imp->SpacingZ();
}


