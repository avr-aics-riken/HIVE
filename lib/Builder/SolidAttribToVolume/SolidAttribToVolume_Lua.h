/**
 * @file SolidAttribToVolume_Lua.h
 * SolidAttribToVolume Luaラッパー
 */
#ifndef _SOLIDATTRIBTOVOLUME_LUA_H_
#define _SOLIDATTRIBTOVOLUME_LUA_H_

#include "LuaUtil.h"
#include "SolidAttribToVolume.h"
#include "BufferVolumeData_Lua.h"
#include "BufferPointData_Lua.h"
/**
 * SolidAttribToVolume Luaラッパー
 */
class SolidAttribToVolume_Lua : public SolidAttribToVolume
{
public:
    SolidAttribToVolume_Lua(){}

    int Create(BufferSolidData_Lua* solid, BufferExtraData_Lua* attrib, float bminX, float bminY, float bminZ, float bmaxX, float bmaxY, float bmaxZ) {
        float bmin[3] = { bminX, bminY, bminZ };
        float bmax[3] = { bmaxX, bmaxY, bmaxZ };
        return SolidAttribToVolume::Create(solid, attrib, bmin, bmax);
    }

    /// Generate volume from solids by interpolating attributes assigned to the vertex of solid.
    bool ToVolume(int w, int h, int d, int mode) {
        return SolidAttribToVolume::ToVolume(w, h, d, mode);
    }

    /// Retrieve volume data.
    /// Valid after ToVolume()
    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(SolidAttribToVolume::VolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(SolidAttribToVolume_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG8(int,Create,BufferSolidData_Lua*,BufferExtraData_Lua*,float,float,float,float,float,float)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG4("SolidAttribToVolume",bool,ToVolume,int,int,int,int)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(SolidAttribToVolume_Lua);

#endif //_SOLIDATTRIBTOVOLUME_LUA_H_

