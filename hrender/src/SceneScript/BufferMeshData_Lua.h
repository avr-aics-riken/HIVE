#ifndef _BUFFERMESHDATA_LUA_H_
#define _BUFFERMESHDATA_LUA_H_

#include "BufferMeshData.h"

class BufferMeshData_Lua : public BufferMeshData
{
	public:
    BufferMeshData_Lua(BufferMeshData* mesh) : BufferMeshData(mesh) { /*this->pos =  mesh->pos;*/ }
    BufferMeshData_Lua(){}
	~BufferMeshData_Lua(){}
    LUA_SCRIPTCLASS_BEGIN(BufferMeshData_Lua)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(BufferMeshData_Lua);


#endif

