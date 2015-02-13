#include "BufferLineData.h"

BufferLineData::BufferLineData()
{
    Clear();
}

BufferLineData::~BufferLineData()
{
    Clear();
}

BufferLineData::BufferLineData(BufferLineData* inst)
{
    this->m_pos    = inst->m_pos;
    this->m_mat    = inst->m_mat;
    this->m_radius = inst->m_radius;
}

void BufferLineData::Clear()
{
    m_pos    = 0;
    m_mat    = 0;
    m_radius = 0;
}

void BufferLineData::Create(int vertexnum)
{
    m_pos    = new Vec3Buffer();
    m_mat    = new FloatBuffer();
    m_radius = new FloatBuffer();
    m_pos->Create(vertexnum);
    m_mat->Create(vertexnum);
    m_radius->Create(vertexnum);
}

Vec3Buffer* BufferLineData::Position() {
    return m_pos;
}

FloatBuffer* BufferLineData::Material() {
    return m_mat;
}

FloatBuffer* BufferLineData::Radius() {
    return m_radius;
}

Vec3Buffer* BufferLineData::Position() const {
    return m_pos;
}

FloatBuffer* BufferLineData::Material() const {
    return m_mat;
}

FloatBuffer* BufferLineData::Radius() const {
    return m_radius;
}


