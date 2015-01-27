#ifndef _STLLOADER_LUA_H_
#define _STLLOADER_LUA_H_


#include "LuaUtil.h"
#include "Buffer.h"
#include "BufferMeshData.h"
#include "StlLoader.h"
#include "BufferMeshData_Lua.h"


//------//------//------//------//------//------//------//------//------
//
//------//------//------//------//------//------//------//------//------
class STLLoader_Lua : public STLLoader
{
public:
	STLLoader_Lua();
	~STLLoader_Lua();
	void Clear();
	bool Load(const char* filename);
	BufferMeshData_Lua* MeshData();
    
	LUA_SCRIPTCLASS_BEGIN(STLLoader_Lua)
	LUA_SCRIPTCLASS_METHOD_ARG1(bool,Load,const char*)
	LUA_SCRIPTCLASS_METHOD_ARG0(BufferMeshData_Lua*, MeshData)
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(STLLoader_Lua);

#endif //_STLLOADER_LUA_H_

