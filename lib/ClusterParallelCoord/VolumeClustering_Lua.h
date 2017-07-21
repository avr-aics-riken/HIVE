/**
 * @file VolumeClustering_Lua.h
 * VolumeClustering Luaラッパー
 */
#ifndef _VOLUMECLUSTERING_LUA_H_
#define _VOLUMECLUSTERING_LUA_H_

#include "LuaUtil.h"
#include "VolumeClustering.h"
#include "BufferVolumeData_Lua.h"

/**
 * VolumeClustering Luaラッパー
 */
class VolumeClustering_Lua : public VolumeClustering
{
public:
    VolumeClustering_Lua() {}
    ~VolumeClustering_Lua() {}

    LuaTable GetClusterValue(int axis, int cluster) {
        LuaTable t;
        VolumeClustering::Cluster c = VolumeClustering::GetClusterValue(axis, cluster);
        t.map("maxValue", static_cast<double>(c.maxValue));
        t.map("minValue", static_cast<double>(c.minValue));
        t.map("topValue", static_cast<double>(c.topValue));
        return t;
    }
   
    LUA_SCRIPTCLASS_BEGIN(VolumeClustering_Lua)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("VolumeClustering", bool, Execute, BufferVolumeData_Lua*)
    LUA_SCRIPTCLASS_METHOD_ARG0(int, GetAxisNum)
    LUA_SCRIPTCLASS_METHOD_ARG1(int, GetClusterNum, int)
    LUA_SCRIPTCLASS_METHOD_ARG2(LuaTable, GetClusterValue, int, int)
    LUA_SCRIPTCLASS_METHOD_ARG3(int, GetEdgePowers, int, int, int)
    LUA_SCRIPTCLASS_METHOD_ARG2(int, SetSigma, int, float)
    LUA_SCRIPTCLASS_METHOD_ARG2(int, SetOrder, int, int)
    LUA_SCRIPTCLASS_METHOD_ARG1(float, GetVolumeMin, int)
    LUA_SCRIPTCLASS_METHOD_ARG1(float, GetVolumeMax, int)
    
    LUA_SCRIPTCLASS_END();

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VolumeClustering_Lua);



#endif //_VOLUMECLUSTERING_LUA_H_

