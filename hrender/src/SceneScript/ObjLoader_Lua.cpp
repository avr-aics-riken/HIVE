#include "ObjLoader_Lua.h"
#include "SimpleObj.h"
#include "BufferMeshData_Lua.h"


OBJLoader_Lua::OBJLoader_Lua(){}
OBJLoader_Lua::~OBJLoader_Lua(){};

bool OBJLoader_Lua::Load(const char* filename){
	return OBJLoader::Load(filename);
}

BufferMeshData_Lua* OBJLoader_Lua::MeshData() {
	return new BufferMeshData_Lua(OBJLoader::MeshData());
}

