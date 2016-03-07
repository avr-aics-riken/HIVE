/**
 * @file SparseVolumeToVolume_Lua.h
 * SparseVolumeToVolume Luaラッパー
 */
#ifndef _SPARSEVOLUMETOVOLUME_LUA_H_
#define _SPARSEVOLUMETOVOLUME_LUA_H_

#include "LuaUtil.h"
#include "SparseVolumeToVolume.h"
#include "BufferVolumeData_Lua.h"
/**
 * SparseVolumeToVolume Luaラッパー
 */
class SparseVolumeToVolume_Lua : public SparseVolumeToVolume
{
public:
    SparseVolumeToVolume_Lua(){}

    int Create(BufferSparseVolumeData_Lua* volume, float resamplingRate = 1.0f) {
        return SparseVolumeToVolume::Create(volume, resamplingRate);
    }

    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(SparseVolumeToVolume::VolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(SparseVolumeToVolume_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG2(int,Create,BufferSparseVolumeData_Lua*,float);
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*, VolumeData)
    LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(SparseVolumeToVolume_Lua);

#endif //_SPARSEVOLUMETOVOLUME_LUA_H_

