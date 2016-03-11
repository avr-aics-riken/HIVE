/**
 * @file CDMLoader_Lua.h
 * CDMLoader Luaラッパー
 */
#ifndef _CDMLOADER_LUA_H_
#define _CDMLOADER_LUA_H_

#include "LuaUtil.h"
#include "CdmLoader.h"
#include "BufferVolumeData_Lua.h"
/**
 * CDMLoader Luaラッパー
 */
class CDMLoader_Lua : public CDMLoader
{
public:
    CDMLoader_Lua() {}
    ~CDMLoader_Lua() {}

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

    double GlobalOffsetX() {
      double buf[3];
      CDMLoader::GlobalOffset(buf);
      return buf[0];
    }

    double GlobalOffsetY() {
      double buf[3];
      CDMLoader::GlobalOffset(buf);
      return buf[1];
    }

    double GlobalOffsetZ() {
      double buf[3];
      CDMLoader::GlobalOffset(buf);
      return buf[2];
    }

    double GlobalRegionX() {
      double buf[3];
      CDMLoader::GlobalRegion(buf);
      return buf[0];
    }

    double GlobalRegionY() {
      double buf[3];
      CDMLoader::GlobalRegion(buf);
      return buf[1];
    }

    double GlobalRegionZ() {
      double buf[3];
      CDMLoader::GlobalRegion(buf);
      return buf[2];
    }

    int GlobalVoxelX() {
      int buf[3];
      CDMLoader::GlobalVoxel(buf);
      return buf[0];
    }

    int GlobalVoxelY() {
      int buf[3];
      CDMLoader::GlobalVoxel(buf);
      return buf[1];
    }

    int GlobalVoxelZ() {
      int buf[3];
      CDMLoader::GlobalVoxel(buf);
      return buf[2];
    }

    int GlobalDivX() {
      int buf[3];
      CDMLoader::GlobalDiv(buf);
      return buf[0];
    }

    int GlobalDivY() {
      int buf[3];
      CDMLoader::GlobalDiv(buf);
      return buf[1];
    }

    int GlobalDivZ() {
      int buf[3];
      CDMLoader::GlobalDiv(buf);
      return buf[2];
    }

    int NumTimeSteps() {
      return CDMLoader::NumTimeSteps();
    }

    int TimeStepAt(int i) {
      return CDMLoader::GetTimeStep(i);
    }

    bool Load(const char* filename, int timeStepIndex) {
        return CDMLoader::Load(filename, timeStepIndex);
    }

    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(CDMLoader::VolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(CDMLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Width)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Height)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Depth)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Component)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,GlobalDivX)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,GlobalDivY)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,GlobalDivZ)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,GlobalVoxelX)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,GlobalVoxelY)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,GlobalVoxelZ)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,NumTimeSteps)
    LUA_SCRIPTCLASS_METHOD_ARG1(int,TimeStepAt,int)
    LUA_SCRIPTCLASS_METHOD_ARG0(double,GlobalOffsetX)
    LUA_SCRIPTCLASS_METHOD_ARG0(double,GlobalOffsetY)
    LUA_SCRIPTCLASS_METHOD_ARG0(double,GlobalOffsetZ)
    LUA_SCRIPTCLASS_METHOD_ARG0(double,GlobalRegionX)
    LUA_SCRIPTCLASS_METHOD_ARG0(double,GlobalRegionY)
    LUA_SCRIPTCLASS_METHOD_ARG0(double,GlobalRegionZ)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG2("CDMLoader",bool,Load,const char*,int)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(CDMLoader_Lua);

#endif //_CDMLOADER_LUA_H_

