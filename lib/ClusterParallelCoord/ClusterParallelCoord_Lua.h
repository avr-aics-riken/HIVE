/**
 * @file ClusterParallelCoord_Lua.h
 * ClusterParallelCoord Luaラッパー
 */
#ifndef _CLUSTERPARALLELCOORD_LUA_H_
#define _CLUSTERPARALLELCOORD_LUA_H_

#include "LuaUtil.h"
#include "VolumeClustering_Lua.h"
#include "VolumeScatterPlot_Lua.h"

/**
 * ClusterParallelCoord Luaラッパー
 */
class ClusterParallelCoord_Lua  : public RefCount
{
private:
	ClusterParallelCoord_Lua(){}
	~ClusterParallelCoord_Lua(){}
public:

    VolumeClustering_Lua* VolumeClustering() {
        return new VolumeClustering_Lua();
    }
    VolumeScatterPlot_Lua* VolumeScatterPlot() {
        return new VolumeScatterPlot_Lua();
    }

    LUA_SCRIPTCLASS_BEGIN(ClusterParallelCoord_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0 (VolumeClustering_Lua*, VolumeClustering)
    LUA_SCRIPTCLASS_METHOD_ARG0 (VolumeScatterPlot_Lua*, VolumeScatterPlot)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(ClusterParallelCoord_Lua);

#endif //_CLUSTERPARALLELCOORD_LUA_H_
