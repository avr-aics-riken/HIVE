#include "StlLoader_Lua.h"
#include "SimpleObj.h"
#include "BufferMeshData_Lua.h"


STLLoader_Lua::STLLoader_Lua(){}
STLLoader_Lua::~STLLoader_Lua(){};

bool STLLoader_Lua::Load(const char* filename){
	return STLLoader::Load(filename);
}

BufferMeshData_Lua* STLLoader_Lua::MeshData() {
	return new BufferMeshData_Lua(STLLoader::MeshData());
}

