/**
 * @file SPHLoader_Lua.h
 * SPHLoader Luaラッパー
 */
#ifndef _SPHLOADER_LUA_H_
#define _SPHLOADER_LUA_H_

#include "LuaUtil.h"
#include "SphLoader.h"
#include "BufferVolumeData_Lua.h"
/**
 * SPHLoader Luaラッパー
 */
class SPHLoader_Lua : public SPHLoader
{
public:
    SPHLoader_Lua() {}
    ~SPHLoader_Lua() {}

    int Width() {
        return SPHLoader::Width();
    }

    int Height() {
        return SPHLoader::Height();
    }

    int Depth() {
        return SPHLoader::Depth();
    }

    int Component() {
        return SPHLoader::Component();
    }

    bool Load(const char* filename) {
        return SPHLoader::Load(filename);
    }

    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(SPHLoader::VolumeData());
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

