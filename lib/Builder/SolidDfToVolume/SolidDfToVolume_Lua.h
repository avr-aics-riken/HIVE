/**
 * @file SolidDfToVolume_Lua.h
 * SolidDfToVolume Luaラッパー
 */
#ifndef _SOLIDDFTOVOLUME_LUA_H_
#define _SOLIDDFTOVOLUME_LUA_H_

#include "LuaUtil.h"
#include "SolidDfToVolume.h"
#include "BufferVolumeData_Lua.h"
#include "BufferPointData_Lua.h"
/**
 * SolidDfToVolume Luaラッパー
 */
class SolidDfToVolume_Lua : public SolidDfToVolume
{
public:
    SolidDfToVolume_Lua(){}

    int Create(BufferSolidData_Lua* solid, float bminX, float bminY, float bminZ, float bmaxX, float bmaxY, float bmaxZ) {
        float bmin[3] = { bminX, bminY, bminZ };
        float bmax[3] = { bmaxX, bmaxY, bmaxZ };
        return SolidDfToVolume::Create(solid, bmin, bmax);
    }

    /// Generate volume representation of solid attribute.
    bool ToVolume(int w, int h, int d) {
        return SolidDfToVolume::ToVolume(w, h, d);
    }

    /// Retrieve volume data.
    /// Valid after ToVolume()
    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(SolidDfToVolume::VolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(SolidDfToVolume_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG7(int,Create,BufferSolidData_Lua*,float,float,float,float,float,float)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG3("SolidDfToVolume",bool,ToVolume,int,int,int)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(SolidDfToVolume_Lua);

#endif //_SOLIDDFTOVOLUME_LUA_H_

