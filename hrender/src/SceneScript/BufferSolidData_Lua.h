/**
 * @file BufferSolidData_Lua.h
 * BufferSolidData Luaラッパー
 */
#ifndef _BUFFERSOLIDDATA_LUA_H_
#define _BUFFERSOLIDDATA_LUA_H_

#include "BufferSolidData.h"
/**
 * BufferSolidData Luaラッパー
 */
class BufferSolidData_Lua : public BufferSolidData
{
public:
    BufferSolidData_Lua(BufferSolidData* vol) : BufferSolidData(vol) { }
    BufferSolidData_Lua() {}
    ~BufferSolidData_Lua() {}
    LUA_SCRIPTCLASS_BEGIN(BufferSolidData_Lua)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(BufferSolidData_Lua);

#endif //_BUFFERSOLIDDATA_LUA_H_

