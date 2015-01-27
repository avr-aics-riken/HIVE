#ifndef _OBJLOADER_LUA_H_
#define _OBJLOADER_LUA_H_


#include "LuaUtil.h"
#include "Buffer.h"
#include "BufferMeshData.h"
#include "ObjLoader.h"
#include "BufferMeshData_Lua.h"


//------//------//------//------//------//------//------//------//------
//
//------//------//------//------//------//------//------//------//------
class OBJLoader_Lua : public OBJLoader
{
public:
	OBJLoader_Lua();
	~OBJLoader_Lua();
	void Clear();
	bool Load(const char* filename);
	BufferMeshData_Lua* MeshData();
    
	LUA_SCRIPTCLASS_BEGIN(OBJLoader_Lua)
	LUA_SCRIPTCLASS_METHOD_ARG1(bool,Load,const char*)
	LUA_SCRIPTCLASS_METHOD_ARG0(BufferMeshData_Lua*, MeshData)
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(OBJLoader_Lua);

#endif //_OBJLOADER_LUA_H_

