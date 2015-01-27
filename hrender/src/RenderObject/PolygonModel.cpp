#include "PolygonModel.h"

PolygonModel::PolygonModel() : RenderObject(TYPE_POLYGON)
{
}

PolygonModel::~PolygonModel()
{
}

bool PolygonModel::SetShader(const std::string& shaderfile)
{
    return false;
}

bool PolygonModel::Create(BufferMeshData *m)
{
    m_mesh   = m;
    return true;
}

BufferMeshData* PolygonModel::GetMesh() const {
    return m_mesh;
}

