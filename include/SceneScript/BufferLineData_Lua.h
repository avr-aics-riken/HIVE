/**
 * @file BufferLineData_Lua.h
 * BufferLineData Luaラッパー
 */
#ifndef _BUFFERLINEDATA_LUA_H_
#define _BUFFERLINEDATA_LUA_H_

#include "BufferLineData.h"

/**
 * BufferLineData Luaラッパー
 */
class BufferLineData_Lua : public BufferLineData
{
protected:
    BufferLineData_Lua(BufferLineData* line) : BufferLineData(line) { }
    BufferLineData_Lua() {}
    ~BufferLineData_Lua() {}

public:
    static BufferLineData_Lua* CreateInstance(BufferLineData* bufferLineData = 0);
    LUA_SCRIPTCLASS_BEGIN(BufferLineData_Lua)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(BufferLineData_Lua);

#endif //_BUFFERLINEDATA_LUA_H_

