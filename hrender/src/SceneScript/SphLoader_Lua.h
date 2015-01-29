#ifndef _SPHLOADER_LUA_H_
#define _SPHLOADER_LUA_H_

#include "LuaUtil.h"
#include "SphLoader.h"
#include "BufferVolumeData_Lua.h"

class SPHLoader_Lua : public SPHLoader
{
public:
    SPHLoader_Lua() {}
    ~SPHLoader_Lua() {}

    int Width() {
        return Width();
    }

    int Height() {
        return Height();
    }

    int Depth() {
        return Depth();
    }

    int Component() {
        return Component();
    }


    bool Load(const char* filename) {
        return Load(filename);
    }

    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(VolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(SPHLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Width)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Height)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Depth)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Component)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool,Load,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(SPHLoader_Lua);

#endif //_SPHLOADER_LUA_H_

