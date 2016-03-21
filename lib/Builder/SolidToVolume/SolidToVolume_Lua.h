/**
 * @file SolidToVolume_Lua.h
 * SolidToVolume Luaラッパー
 */
#ifndef _SOLIDTOVOLUME_LUA_H_
#define _SOLIDTOVOLUME_LUA_H_

#include "LuaUtil.h"
#include "SolidToVolume.h"
#include "BufferVolumeData_Lua.h"
/**
 * SolidToVolume Luaラッパー
 */
class SolidToVolume_Lua : public SolidToVolume
{
public:
    SolidToVolume_Lua(){}

    int Create(BufferSolidData_Lua* mesh, float bminX, float bminY, float bminZ, float bmaxX, float bmaxY, float bmaxZ) {
        float bmin[3] = { bminX, bminY, bminZ };
        float bmax[3] = { bmaxX, bmaxY, bmaxZ };
        return SolidToVolume::Create(mesh, bmin, bmax);
    }

    /// Generate volume representation of mesh attribute.
    bool ToVolume(int w, int h, int d) {
        return SolidToVolume::ToVolume(w, h, d);
    }

    /// Retrieve volume data.
    /// Valid after ToVolume()
    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(SolidToVolume::VolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(SolidToVolume_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG7(int,Create,BufferSolidData_Lua*,float,float,float,float,float,float)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG3("SolidToVolume",bool,ToVolume,int,int,int)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(SolidToVolume_Lua);

#endif //_SOLIDTOVOLUME_LUA_H_

