/**
 * @file VolumeToVector_Lua.h
 * VolumeToVector Luaラッパー
 */
#ifndef _VOLUMETOVECTOR_LUA_H_
#define _VOLUMETOVECTOR_LUA_H_

#include "LuaUtil.h"
#include "BufferVolumeData_Lua.h"
#include "VolumeQuantizer.h"
/**
 * VolumeQuantizer Luaラッパー
 */
class VolumeQuantizer_Lua : public VolumeQuantizer
{
public:
    VolumeQuantizer_Lua(){}

    int Create(BufferVolumeData_Lua* volume) {
        return VolumeQuantizer::Create(volume);
    }

    BufferVolumeData_Lua* VolumeData() {
        RefPtr<BufferVolumeData> data = VolumeQuantizer::VolumeData();
        return BufferVolumeData_Lua::CreateInstance(data);
    }

    LUA_SCRIPTCLASS_BEGIN(VolumeQuantizer_Lua)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("VolumeQuantizer",int,Create,BufferVolumeData_Lua*);
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*, VolumeData)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, QuantizeSize, int)
    LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VolumeQuantizer_Lua);

#endif //_VOLUMETOVECTOR_LUA_H_

