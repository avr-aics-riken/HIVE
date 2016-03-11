/**
 * @file VOLLoader_Lua.h
 * VOLLoader_Lua Luaラッパー
 */
#ifndef _VOLLOADER_LUA_H_
#define _VOLLOADER_LUA_H_

#include "LuaUtil.h"
#include "VolLoader.h"
#include "BufferVolumeData_Lua.h"
/**
 * VOLLoader_Lua Luaラッパー
 */
class VOLLoader_Lua : public VOLLoader
{
public:
    VOLLoader_Lua() {}
    ~VOLLoader_Lua() {}

    void Clear() {
        VOLLoader::Clear();
    }

    bool Load(const char* filename) {
        return VOLLoader::Load(filename);
    }

    int Width() {
        return VOLLoader::Width();
    }

    int Height() {
        return VOLLoader::Height();
    }

    int Depth() {
        return VOLLoader::Depth();
    }

    int Component() {
        return VOLLoader::Component();
    }

    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(VOLLoader::VolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(VOLLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Width)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Height)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Depth)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Component)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("VOLLoader",bool,Load,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VOLLoader_Lua);

#endif //_VOLLOADER_LUA_H_

