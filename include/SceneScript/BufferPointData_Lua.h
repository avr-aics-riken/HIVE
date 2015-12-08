/**
 * @file BufferPointData_Lua.h
 * BufferPointData Luaラッパー
 */
#ifndef _BUFFERPOINTDATA_LUA_H_
#define _BUFFERPOINTDATA_LUA_H_

#include "BufferPointData.h"

/**
 * BufferPointData Luaラッパー
 */
class BufferPointData_Lua : public BufferPointData
{
protected:
    BufferPointData_Lua(BufferPointData* pnt) : BufferPointData(pnt) { }
    BufferPointData_Lua() {}
    ~BufferPointData_Lua() {}
    
public:
    static BufferPointData_Lua* CreateInstance(BufferPointData* bufferPointData = 0);
    LUA_SCRIPTCLASS_BEGIN(BufferPointData_Lua)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(BufferPointData_Lua);

#endif //_BUFFERPOINTDATA_LUA_H_

