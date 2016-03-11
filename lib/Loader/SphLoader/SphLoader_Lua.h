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

    float OriginX() {
      float origin[3];
      SPHLoader::Origin(origin);
      return origin[0];
    }

    float OriginY() {
      float origin[3];
      SPHLoader::Origin(origin);
      return origin[1];
    }

    float OriginZ() {
      float origin[3];
      SPHLoader::Origin(origin);
      return origin[2];
    }

    float PitchX() {
      float pitch[3];
      SPHLoader::Pitch(pitch);
      return pitch[0];
    }

    float PitchY() {
      float pitch[3];
      SPHLoader::Pitch(pitch);
      return pitch[1];
    }

    float PitchZ() {
      float pitch[3];
      SPHLoader::Pitch(pitch);
      return pitch[2];
    }

    float Time() {
      return SPHLoader::Time();
    }

    float Step() {
      return SPHLoader::Step();
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
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("SPHLoader",bool,Load,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_METHOD_ARG0(float,PitchX)
    LUA_SCRIPTCLASS_METHOD_ARG0(float,PitchY)
    LUA_SCRIPTCLASS_METHOD_ARG0(float,PitchZ)
    LUA_SCRIPTCLASS_METHOD_ARG0(float,OriginX)
    LUA_SCRIPTCLASS_METHOD_ARG0(float,OriginY)
    LUA_SCRIPTCLASS_METHOD_ARG0(float,OriginZ)
    LUA_SCRIPTCLASS_METHOD_ARG0(float,Time)
    LUA_SCRIPTCLASS_METHOD_ARG0(float,Step)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(SPHLoader_Lua);

#endif //_SPHLOADER_LUA_H_

