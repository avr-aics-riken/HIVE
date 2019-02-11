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
#include "BufferExtraData_Lua.h"

/**
 * PointToVolume Luaラッパー
 */
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

    ///
    /// Rasterizer particle attribute to voxel.
    /// Supports voxelize vec3 and scalar data.
    /// Create a scalar valued voxel for vec3 data when `vector_length` is true
    /// (take a length of vector value)
    /// 
    bool ToVolumeWithAttrib(int w, int h, int d, BufferExtraData_Lua *attrib, bool vector_length = false) {
        return PointToVolume::ToVolumeWithAttrib(w, h, d, attrib, vector_length);
    }

    /// Retrieve volume data.
    /// Valid after ToVolume()
    BufferVolumeData_Lua* VolumeData() {
        RefPtr<BufferVolumeData> data = PointToVolume::VolumeData();
        return BufferVolumeData_Lua::CreateInstance(data);
    }

    LUA_SCRIPTCLASS_BEGIN(PointToVolume_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(int,Create,BufferPointData_Lua*)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG3("PointToVolume",bool,ToVolume,int,int,int)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG5("PointToVolumeWithAttrib",bool,ToVolumeWithAttrib,int,int,int,BufferExtraData_Lua*, bool)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(PointToVolume_Lua);

#endif //_POINTTOVOLUME_LUA_H_

