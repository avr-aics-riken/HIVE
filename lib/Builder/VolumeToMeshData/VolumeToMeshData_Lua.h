/**
 * @file VolumeToMeshData_Lua.h
 * VolumeToMeshData Luaラッパー
 */
#ifndef _VOLUMETOMESHDATA_LUA_H_
#define _VOLUMETOMESHDATA_LUA_H_

#include "LuaUtil.h"
#include "VolumeToMeshData.h"
#include "BufferVolumeData_Lua.h"
#include "BufferMeshData_Lua.h"
/**
 * VolumeToMeshData Luaラッパー
 */
class VolumeToMeshData_Lua : public VolumeToMeshData
{
public:
    VolumeToMeshData_Lua(){}

    int Create(BufferVolumeData_Lua* volume) {
        return VolumeToMeshData::Create(volume);
    }

    bool SetIsoValue(float isovalue) {
        VolumeToMeshData::SetIsoValue(static_cast<double>(isovalue));
        return true;
    }

    /// Generate isosurface mesh.
    /// Valid after SetIsoValue()
    int IsoSurface() {
        return VolumeToMeshData::IsoSurface();
    }

    /// Retrieve isosurface mesh.
    /// Valid after IsoSurface()
    BufferMeshData_Lua* MeshData() {
        RefPtr<BufferMeshData> mesh = VolumeToMeshData::MeshData();
        return BufferMeshData_Lua::CreateInstance(mesh);
    }

    LUA_SCRIPTCLASS_BEGIN(VolumeToMeshData_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(int,Create,BufferVolumeData_Lua*)
    LUA_SCRIPTCLASS_METHOD_ARG1(int,SetIsoValue, float)
    LUA_SCRIPTCLASS_METHOD_ARG0(int, IsoSurface)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferMeshData_Lua*,MeshData)
    LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VolumeToMeshData_Lua);

#endif //_VOLUMETOMESHDATA_LUA_H_

