#include "PolygonModel.h"

PolygonModel::PolygonModel() : RenderObject(TYPE_POLYGON)
{
}

PolygonModel::~PolygonModel()
{
}

bool PolygonModel::SetShader(const std::string& shaderfile)
{
    m_shaderfile = shaderfile;
    return true;
}

bool PolygonModel::Create(BufferMeshData *m)
{
    m_mesh   = m;
    return true;
}

BufferMeshData* PolygonModel::GetMesh() const {
    return m_mesh;
}

const std::string& PolygonModel::GetShader() const
{
    return m_shaderfile;
}

