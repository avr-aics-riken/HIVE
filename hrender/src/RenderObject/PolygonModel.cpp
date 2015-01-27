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

bool PolygonModel::Create(BufferMeshData *m)
{
	if(m) m->print();
	else  printf("%s:Mesh Data is NULL\n", __func__);
	SetMesh(m);
	return true;
}

