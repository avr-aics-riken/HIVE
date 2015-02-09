#include "BufferVolumeData.h"

BufferVolumeData::BufferVolumeData()
{
    Clear();
}

BufferVolumeData::~BufferVolumeData()
{
    Clear();
}

BufferVolumeData::BufferVolumeData(BufferVolumeData* inst)
{
    this->m_dim[0] = inst->m_dim[0];
    this->m_dim[1] = inst->m_dim[1];
    this->m_dim[2] = inst->m_dim[2];
    this->m_comp   = inst->m_comp;
    this->m_buffer = inst->m_buffer;
}

void BufferVolumeData::Create(int w, int h, int d, int component)
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
}

void BufferVolumeData::Clear()
{
    m_dim[0] = m_dim[1] = m_dim[2] = 0;
    m_comp   = 0;
    m_buffer = 0;
}

void BufferVolumeData::print()
{
}

int BufferVolumeData::Width() {
    return m_dim[0];
}

int BufferVolumeData::Height() {
    return m_dim[1];
}

int BufferVolumeData::Depth() {
    return m_dim[2];
}

int BufferVolumeData::Component() {
    return m_comp;
}

FloatBuffer *BufferVolumeData::Buffer() {
    return m_buffer;
}

