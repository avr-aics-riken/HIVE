/**
 * @file RawVolumeLoader_Lua.h
 * RawVolumeLoader_Lua Luaラッパー
 */
#ifndef _RAWVOLUMELOADER_LUA_H_
#define _RAWVOLUMELOADER_LUA_H_

#include "LuaUtil.h"
#include "RawVolumeLoader.h"
#include "BufferVolumeData_Lua.h"
/**
 * RawVolumeLoader_Lua Luaラッパー
 */
class RawVolumeLoader_Lua : public RawVolumeLoader
{
public:
    RawVolumeLoader_Lua() {}
    ~RawVolumeLoader_Lua() {}

    void Clear() {
        RawVolumeLoader::Clear();
    }

    bool Load(const char* filename, int width, int height, int depth, int components, const char* format) {
        return RawVolumeLoader::Load(filename, width, height, depth, components, format);
    }

    int Width() {
        return RawVolumeLoader::Width();
    }

    int Height() {
        return RawVolumeLoader::Height();
    }

    int Depth() {
        return RawVolumeLoader::Depth();
    }

    int Component() {
        return RawVolumeLoader::Component();
    }

    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(RawVolumeLoader::VolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(RawVolumeLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Width)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Height)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Depth)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Component)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG6("RawVolumeLoader", bool,Load,const char*,int,int,int,int,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(RawVolumeLoader_Lua);

#endif //_RAWVOLUMELOADER_LUA_H_
