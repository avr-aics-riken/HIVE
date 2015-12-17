/**
 * @file MeshAttribToVolume_Lua.h
 * MeshAttribToVolume Luaラッパー
 */
#ifndef _MESHATTRIBTOVOLUME_LUA_H_
#define _MESHATTRIBTOVOLUME_LUA_H_

#include "LuaUtil.h"
#include "MeshAttribToVolume.h"
#include "BufferVolumeData_Lua.h"
#include "BufferPointData_Lua.h"
/**
 * MeshAttribToVolume Luaラッパー
 */
class MeshAttribToVolume_Lua : public MeshAttribToVolume
{
public:
    MeshAttribToVolume_Lua(){}

    int Create(BufferMeshData_Lua* mesh, BufferExtraData_Lua* attrib, float bminX, float bminY, float bminZ, float bmaxX, float bmaxY, float bmaxZ) {
        float bmin[3] = { bminX, bminY, bminZ };
        float bmax[3] = { bmaxX, bmaxY, bmaxZ };
        return MeshAttribToVolume::Create(mesh, attrib, bmin, bmax);
    }

    /// Generate volume representation of mesh attribute.
    bool ToVolume(int w, int h, int d) {
        return MeshAttribToVolume::ToVolume(w, h, d);
    }

    /// Retrieve volume data.
    /// Valid after ToVolume()
    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(MeshAttribToVolume::VolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(MeshAttribToVolume_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG8(int,Create,BufferMeshData_Lua*,BufferExtraData_Lua*,float,float,float,float,float,float)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool,ToVolume,int,int,int)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(MeshAttribToVolume_Lua);

#endif //_MESHATTRIBTOVOLUME_LUA_H_

