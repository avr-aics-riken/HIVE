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

void BufferVolumeData::Clear()
{
    m_dim[0] = m_dim[1] = m_dim[2] = 0;
    m_comp = 0;
    m_buffer = 0;
}

void BufferVolumeData::print()
{
    /*	printf("%s:Position() %08p\n", __func__, m_pos.Get()      );
        printf("%s:Normal()   %08X\n", __func__, m_normal.Get()   );
        printf("%s:Texcoord() %08X\n", __func__, m_texcoord.Get() );
        printf("%s:Index()    %08X\n", __func__, m_index.Get()    );
        printf("%s:Material() %08X\n", __func__, m_mat.Get()      );*/
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

