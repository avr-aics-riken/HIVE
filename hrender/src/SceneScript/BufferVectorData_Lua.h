/**
 * @file BufferVectorData_Lua.h
 * BufferVectorData Luaラッパー
 */
#ifndef _BUFFERVECTORDATA_LUA_H_
#define _BUFFERVECTORDATA_LUA_H_

#include "BufferVectorData.h"

class BufferVectorData_Lua : public BufferVectorData
{
public:
    BufferVectorData_Lua(BufferVectorData* vec) : BufferVectorData(vec) { }
    BufferVectorData_Lua() {}
    ~BufferVectorData_Lua() {}
    LUA_SCRIPTCLASS_BEGIN(BufferVectorData_Lua)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(BufferVectorData_Lua);

#endif //_BUFFERVECTORDATA_LUA_H_

