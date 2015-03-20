/**
 * @file BufferImageData_Lua.h
 * BufferImageData Luaラッパー
 */
#ifndef _BUFFERIMAGEDATA_LUA_H_
#define _BUFFERIMAGEDATA_LUA_H_

#include "BufferImageData.h"

/**
 * BufferImageData Luaラッパー
 */
class BufferImageData_Lua : public BufferImageData
{
public:
    BufferImageData_Lua(BufferImageData* data) : BufferImageData(data) { }
    BufferImageData_Lua(const BufferImageData* data) : BufferImageData(data) { }
    BufferImageData_Lua() {}
    ~BufferImageData_Lua() {}
    LUA_SCRIPTCLASS_BEGIN(BufferImageData_Lua)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(BufferImageData_Lua);

#endif //_BUFFERIMAGEDATA_LUA_H_

