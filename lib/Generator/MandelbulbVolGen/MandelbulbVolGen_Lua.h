/**
 * @file VOLLoader_Lua.h
 * VOLLoader_Lua Luaラッパー
 */
#ifndef _MANDELBULBVOLGEN_LUA_H_
#define _MANDELBULBVOLGEN_LUA_H_

#include "LuaUtil.h"
#include "MandelbulbVolGen.h"
#include "BufferVolumeData_Lua.h"
/**
 * MandelbulbVolGen_Lua Luaラッパー
 */
class MandelbulbVolGen_Lua : public MandelbulbVolGen
{
public:
    MandelbulbVolGen_Lua() {}
    ~MandelbulbVolGen_Lua() {}

    void Clear() {
        MandelbulbVolGen::Clear();
    }

    bool Generate(const int w, const int h, const int d, const int c) {
        return MandelbulbVolGen::Generate(w, h, d, c);
    }

    int Width() {
        return MandelbulbVolGen::Width();
    }

    int Height() {
        return MandelbulbVolGen::Height();
    }

    int Depth() {
        return MandelbulbVolGen::Depth();
    }

    int Component() {
        return MandelbulbVolGen::Component();
    }

    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(MandelbulbVolGen::VolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(MandelbulbVolGen_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Width)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Height)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Depth)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Component)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG4("MandelbulbVolGen",bool,Generate,
                                             const int, const int, const int, const int)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(MandelbulbVolGen_Lua);

#endif //_MANDELBULBVOLGEN_LUA_H_

