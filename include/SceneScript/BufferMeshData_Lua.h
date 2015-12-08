/**
 * @file BufferMeshData_Lua.h
 * BufferMeshData Luaラッパー
 */
#ifndef _BUFFERMESHDATA_LUA_H_
#define _BUFFERMESHDATA_LUA_H_

#include "BufferMeshData.h"

/**
 * BufferMeshData Luaラッパー
 */
class BufferMeshData_Lua : public BufferMeshData
{
protected:
    BufferMeshData_Lua(BufferMeshData* mesh) : BufferMeshData(mesh) { }
    BufferMeshData_Lua(){}
	~BufferMeshData_Lua(){}
    
public:
    static BufferMeshData_Lua* CreateInstance(BufferMeshData* bufferMeshData = 0);
    LUA_SCRIPTCLASS_BEGIN(BufferMeshData_Lua)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(BufferMeshData_Lua);


#endif

