/**
 * @file VTKLoader_Lua.h
 * VTKLoader Luaラッパー
 */
#ifndef _VTKLOADER_LUA_H_
#define _VTKLOADER_LUA_H_

#include "LuaUtil.h"
#include "VtkLoader.h"
#include "BufferVolumeData_Lua.h"
/**
 * VTKLoader Luaラッパー
 */
class VTKLoader_Lua : public VTKLoader
{
public:
    VTKLoader_Lua() {}
    ~VTKLoader_Lua() {}

    int Width() {
        return VTKLoader::Width();
    }

    int Height() {
        return VTKLoader::Height();
    }

    int Depth() {
        return VTKLoader::Depth();
    }

    int Component() {
        return VTKLoader::Component();
    }

    float OriginX() {
      float origin[3];
      VTKLoader::Origin(origin);
      return origin[0];
    }

    float OriginY() {
      float origin[3];
      VTKLoader::Origin(origin);
      return origin[1];
    }

    float OriginZ() {
      float origin[3];
      VTKLoader::Origin(origin);
      return origin[2];
    }

    float PitchX() {
      float pitch[3];
      VTKLoader::Pitch(pitch);
      return pitch[0];
    }

    float PitchY() {
      float pitch[3];
      VTKLoader::Pitch(pitch);
      return pitch[1];
    }

    float PitchZ() {
      float pitch[3];
      VTKLoader::Pitch(pitch);
      return pitch[2];
    }

    float Time() {
      return VTKLoader::Time();
    }

    float Step() {
      return VTKLoader::Step();
    }

    bool Load(const char* filename) {
        return VTKLoader::Load(filename);
    }

    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(VTKLoader::VolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(VTKLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Width)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Height)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Depth)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Component)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool,Load,const char*)
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
LUA_SCRIPTCLASS_CAST_AND_PUSH(VTKLoader_Lua);

#endif //_VTKLOADER_LUA_H_

