#include "PolygonModel_Lua.h"

PolygonModel_Lua::PolygonModel_Lua()
{
}

PolygonModel_Lua::~PolygonModel_Lua()
{
}

bool PolygonModel_Lua::SetShader(const std::string& shaderfile)
{
    return false;
}

bool PolygonModel_Lua::Create(BufferMeshData *mesh)
{
    printf("Mesh Data=%08X\n", mesh);
    if(mesh) mesh->print();
    else     printf("MESH DATA is NULL\n");
    PolygonModel::Create(mesh);
    return true;
}

