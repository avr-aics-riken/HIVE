/**
 * @file PointToVolume_Lua.h
 * PointToVolume Luaラッパー
 */
#ifndef _POINTTOVOLUME_LUA_H_
#define _POINTTOVOLUME_LUA_H_

#include "LuaUtil.h"
#include "PointToVolume.h"
#include "BufferVolumeData_Lua.h"
#include "BufferPointData_Lua.h"

class PointToVolume_Lua : public PointToVolume
{
public:
    PointToVolume_Lua(){}

    int Create(BufferPointData_Lua* point) {
        return PointToVolume::Create(point);
    }

    /// Generate volume representation of particles.
    bool ToVolume(int w, int h, int d) {
        return PointToVolume::ToVolume(w, h, d);
    }

    /// Retrieve volume data.
    /// Valid after ToVolume()
    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(PointToVolume::VolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(PointToVolume_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(int,Create,BufferPointData_Lua*)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool,ToVolume,int,int,int)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(PointToVolume_Lua);

#endif //_POINTTOVOLUME_LUA_H_

