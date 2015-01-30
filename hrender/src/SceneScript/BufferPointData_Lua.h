#ifndef _BUFFERPOINTDATA_LUA_H_
#define _BUFFERPOINTDATA_LUA_H_

#include "BufferPointData.h"

class BufferPointData_Lua : public BufferPointData
{
public:
    BufferPointData_Lua(BufferPointData* vol) : BufferPointData(vol) { }
    BufferPointData_Lua() {}
    ~BufferPointData_Lua() {}
    LUA_SCRIPTCLASS_BEGIN(BufferPointData_Lua)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(BufferPointData_Lua);

#endif //_BUFFERPOINTDATA_LUA_H_

