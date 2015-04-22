/**
 * @file VolumeFilter_Lua.h
 * VolumeFilter Luaラッパー
 */
#ifndef _VOLUMEFILTER_LUA_H_
#define _VOLUMEFILTER_LUA_H_

#include "LuaUtil.h"
#include "VolumeFilter.h"

/**
 * VolumeFilter Luaラッパー
 */
class VolumeFilter_Lua : public VolumeFilter
{
public:
    VolumeFilter_Lua(){}

    int Norm(BufferVolumeData_Lua* volume) {
        return VolumeFilter::Norm(volume);
    }

    int Laplacian(BufferVolumeData_Lua* volume) {
        return VolumeFilter::Laplacian(volume);
    }

    int Curl(BufferVolumeData_Lua* volume) {
        return VolumeFilter::Curl(volume);
    }

    int Gradient(BufferVolumeData_Lua* volume) {
        return VolumeFilter::Gradient(volume);
    }

    int CompileOption(const char* compiler, const char* compileOpts) {
        return VolumeFilter::SetCompileOption(compiler, compileOpts);
    }

    int Expr(BufferVolumeData_Lua* volume0, BufferVolumeData_Lua* volume1, BufferVolumeData_Lua* volume2, BufferVolumeData_Lua* volume3, int targetNumberOfComponents, const char* source) {
        return VolumeFilter::Expr(volume0, volume1, volume2, volume3, targetNumberOfComponents, source);
    }

    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(VolumeFilter::VolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(VolumeFilter_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(int,Norm,BufferVolumeData_Lua*);
    LUA_SCRIPTCLASS_METHOD_ARG1(int,Laplacian,BufferVolumeData_Lua*);
    LUA_SCRIPTCLASS_METHOD_ARG1(int,Curl,BufferVolumeData_Lua*);
    LUA_SCRIPTCLASS_METHOD_ARG1(int,Gradient,BufferVolumeData_Lua*);
    LUA_SCRIPTCLASS_METHOD_ARG2(int,CompileOption,const char*,const char*);
    LUA_SCRIPTCLASS_METHOD_ARG6(int,Expr,BufferVolumeData_Lua*,BufferVolumeData_Lua*,BufferVolumeData_Lua*,BufferVolumeData_Lua*,int,const char*);
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*, VolumeData)
    LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VolumeFilter_Lua);

#endif //_VOLUMEFILTER_LUA_H_

