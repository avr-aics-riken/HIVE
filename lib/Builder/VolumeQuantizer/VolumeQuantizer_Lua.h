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

    bool Add(BufferVolumeData_Lua* volume) {
        return VolumeQuantizer::Add(volume);
    }

    BufferVolumeData_Lua* VolumeData() {
        RefPtr<BufferVolumeData> data = VolumeQuantizer::VolumeData();
        return BufferVolumeData_Lua::CreateInstance(data);
    }
    
    LuaTable GetMinMax() {
        const std::vector<std::pair<float, float> >& minmax = VolumeQuantizer::GetMinMax();
        LuaTable t;
        LuaTable mm;
        for (size_t i = 0; i < minmax.size(); ++i) {
            mm.map("min", minmax[i].first);
            mm.map("max", minmax[i].second);
            t.push(mm);
        }
        return t;
    }


    LUA_SCRIPTCLASS_BEGIN(VolumeQuantizer_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool,Add,BufferVolumeData_Lua*)
    LUA_SCRIPTCLASS_METHOD_ARG0(bool,Clear)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*, VolumeData)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, QuantizeSize, int)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SamplingNum, int, int, int)
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetMinMax)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG0("VolumeQuantizer",int,Create)

    LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VolumeQuantizer_Lua);

#endif //_VOLUMETOVECTOR_LUA_H_

