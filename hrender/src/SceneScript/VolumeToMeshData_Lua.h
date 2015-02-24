#ifndef _VOLUMETOMESHDATA_LUA_H_
#define _VOLUMETOMESHDATA_LUA_H_

#include "LuaUtil.h"
#include "VolumeToMeshData.h"
#include "BufferVolumeData_Lua.h"
#include "BufferMeshData_Lua.h"

class VolumeToMeshData_Lua : public VolumeToMeshData
{
public:
    VolumeToMeshData_Lua(){}

    int Create(BufferVolumeData_Lua* volume, float isovalue) {
        return VolumeToMeshData::Create(volume, static_cast<double>(isovalue));
    }

    BufferMeshData_Lua* MeshData() {
        return new BufferMeshData_Lua(VolumeToMeshData::MeshData());
    }

    LUA_SCRIPTCLASS_BEGIN(VolumeToMeshData_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG2(int,Create,BufferVolumeData_Lua*, float)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferMeshData_Lua*,MeshData)
    LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VolumeToMeshData_Lua);

#endif //_VOLUMETOMESHDATA_LUA_H_

