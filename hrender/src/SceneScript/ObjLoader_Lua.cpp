#include "ObjLoader_Lua.h"
#include "SimpleObj.h"


OBJLoader_Lua::OBJLoader_Lua(){}
OBJLoader_Lua::~OBJLoader_Lua(){};

bool OBJLoader_Lua::Load(const char* filename){
	return OBJLoader::Load(filename);
}

BufferMeshData  OBJLoader_Lua::MeshData() {
	return OBJLoader::MeshData();
}

