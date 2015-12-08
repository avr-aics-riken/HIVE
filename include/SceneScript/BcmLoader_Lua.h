/**
 * @file BcmLoader_Lua.h
 * BcmローダLuaラッパー
 */
#ifndef _BCMLOADER_LUA_H_
#define _BCMLOADER_LUA_H_

#include "LuaUtil.h"
#include "BcmLoader.h"
#include "BufferVolumeData_Lua.h"

/**
 * BcmローダLuaラッパー
 */
class BCMLoader_Lua : public BCMLoader
{
public:
    BCMLoader_Lua() {}
    ~BCMLoader_Lua() {}

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
        return BCMLoader::Load(filename);
    }

    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(BCMLoader::VolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(BCMLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Width)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Height)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Depth)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Component)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool,Load,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(BCMLoader_Lua);

#endif //_BCMLOADER_LUA_H_

