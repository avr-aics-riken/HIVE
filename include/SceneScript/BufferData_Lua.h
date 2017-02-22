/**
 * @file BufferData_Lua.h
 * BufferExtraData Luaラッパー
 */
#ifndef _BUFFERDATA_LUA_H_
#define _BUFFERDATA_LUA_H_

#include "BufferData.h"

/**
 * BufferData Luaラッパー
 */
class BufferData_Lua : public BufferData
{
protected:
    BufferData_Lua() {}
    ~BufferData_Lua() {}
    
public:
    LUA_SCRIPTCLASS_BEGIN(BufferData_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(int, GetType)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(BufferData_Lua);

#endif //_BUFFERDATA_LUA_H_

