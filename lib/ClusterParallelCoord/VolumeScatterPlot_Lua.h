/**
 * @file VolumeScatterPlot_Lua.h
 * VolumeScatterPlot Luaラッパー
 */
#ifndef _VOLUMESCATTERPLOTLUA_H_
#define _VOLUMESCATTERPLOTLUA_H_

#include "LuaUtil.h"
#include "VolumeScatterPlot.h"
#include "BufferVolumeData_Lua.h"
#include "BufferImageData_Lua.h"

/**
 * VolumeScatterPlot Luaラッパー
 */
class VolumeScatterPlot_Lua : public VolumeScatterPlot
{
public:
    VolumeScatterPlot_Lua() {}
    ~VolumeScatterPlot_Lua() {}
   
    BufferImageData_Lua* GetImageBuffer() {
        return new BufferImageData_Lua(VolumeScatterPlot::GetImageBuffer());
    }
    
    LUA_SCRIPTCLASS_BEGIN(VolumeScatterPlot_Lua)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG3("VolumeScatterPlot", bool, Execute, BufferVolumeData_Lua*, int, int)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferImageData_Lua*, GetImageBuffer);
    LUA_SCRIPTCLASS_END();

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VolumeScatterPlot_Lua);

#endif //_VOLUMESCATTERPLOTLUA_H_
