#include "PolygonModel.h"

PolygonModel::PolygonModel()
{
	SetType(TYPE_POLYGON);
}

PolygonModel::~PolygonModel()
{
}

bool PolygonModel::SetShader(const std::string& shaderfile)
{
	return false;
}

bool PolygonModel::Create(BufferMeshData &mesh)
{
	SetMesh(mesh);
	return true;
}

