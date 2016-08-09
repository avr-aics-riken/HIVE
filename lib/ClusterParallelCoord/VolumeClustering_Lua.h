/**
 * @file VolumeClusterAnalyzer_Lua.h
 * VolumeClusterAnalyzer Luaラッパー
 */
#ifndef _VOLUMECLUSTERING_LUA_H_
#define _VOLUMECLUSTERING_LUA_H_

#include "LuaUtil.h"
#include "VolumeClustering.h"
#include "BufferVolumeData_Lua.h"

/**
 * VolumeClusterAnalyzer Luaラッパー
 */
class VolumeClustering_Lua : public VolumeClustering
{
public:
    VolumeClustering_Lua() {}
    ~VolumeClustering_Lua() {}
   
    LUA_SCRIPTCLASS_BEGIN(VolumeClustering_Lua)
    //LUA_SCRIPTCLASS_METHOD_ARG0(double, MinX)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("VolumeClustering", bool, Execute, BufferVolumeData_Lua*)
    LUA_SCRIPTCLASS_END();

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VolumeClustering_Lua);


#endif //_VOLUMECLUSTERING_LUA_H_

