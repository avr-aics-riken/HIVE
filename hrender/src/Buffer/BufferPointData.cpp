#include "BufferPointData.h"

BufferPointData::BufferPointData()
{
    Clear();
}

BufferPointData::~BufferPointData()
{
    Clear();
}

BufferPointData::BufferPointData(BufferPointData* inst)
{
    this->m_pos    = inst->m_pos;
    this->m_mat    = inst->m_mat;
    this->m_radius = inst->m_radius;
}

void BufferPointData::Clear()
{
    m_pos    = 0;
    m_mat    = 0;
    m_radius = 0;
}

void BufferPointData::Create(int vertexnum)
{
    m_pos    = new Vec3Buffer();
    m_mat    = new FloatBuffer();
    m_radius = new FloatBuffer();
    m_pos->Create(vertexnum);
    m_mat->Create(vertexnum);
    m_radius->Create(vertexnum);
}

Vec3Buffer* BufferPointData::Position() {
    return m_pos;
}

FloatBuffer* BufferPointData::Material() {
    return m_mat;
}

FloatBuffer* BufferPointData::Radius() {
    return m_radius;
}

Vec3Buffer* BufferPointData::Position() const {
    return m_pos;
}

FloatBuffer* BufferPointData::Material() const {
    return m_mat;
}

FloatBuffer* BufferPointData::Radius() const {
    return m_radius;
}


