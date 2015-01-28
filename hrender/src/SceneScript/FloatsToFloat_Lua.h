#ifndef _FLOATSTOFLOAT_LUA_H_
#define _FLOATSTOFLOAT_LUA_H_

#include "LuaUtil.h"
#include "FloatsToFloat.h"
#include "BufferVolumeData_Lua.h"

class FloatsToFloat_Lua : public FloatsToFloat
{
public:
    FloatsToFloat_Lua();
    int Create(BufferVolumeData_Lua* volume, int offset);
    int Component();
    BufferVolumeData_Lua* VolumeData();
    LUA_SCRIPTCLASS_BEGIN(FloatsToFloat_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG2(int,Create,BufferVolumeData_Lua*, int)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Component)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(FloatsToFloat_Lua);

#endif //_FLOATSTOFLOAT_LUA_H_

