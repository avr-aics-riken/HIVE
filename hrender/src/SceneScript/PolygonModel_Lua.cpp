#include "PolygonModel_Lua.h"

PolygonModel_Lua::PolygonModel_Lua()
{
}

PolygonModel_Lua::~PolygonModel_Lua()
{
}

bool PolygonModel_Lua::SetShader(const std::string& shaderfile)
{
    return PolygonModel::SetShader(shaderfile);
}

bool PolygonModel_Lua::Create(BufferMeshData_Lua *mesh)
{
    PolygonModel::Create(mesh);
    return true;
}

