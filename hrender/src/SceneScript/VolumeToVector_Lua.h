#ifndef _VOLUMETOVECTOR_LUA_H_
#define _VOLUMETOVECTOR_LUA_H_

#include "LuaUtil.h"
#include "VolumeToVector.h"
#include "BufferLineData_Lua.h"
#include "BufferTetraData_Lua.h"

class VolumeToVector_Lua : public VolumeToVector
{
public:
    VolumeToVector_Lua(){}

    int Create(BufferVolumeData_Lua* volume, double scale, double radius_scale) {
        return VolumeToVector::Create(volume, scale, radius_scale);
    }

    BufferLineData_Lua* LineData() {
        return new BufferLineData_Lua(VolumeToVector::LineData());
    }

    BufferTetraData_Lua* TetraData() {
        return new BufferTetraData_Lua(VolumeToVector::TetraData());
    }

    bool DividePitchX(double a) {
        return VolumeToVector::DividePitchX(a);
    }
    bool DividePitchY(double a) {
        return VolumeToVector::DividePitchY(a);
    }
    bool DividePitchZ(double a) {
        return VolumeToVector::DividePitchZ(a);
    }

    bool DividePitch(double x, double y, double z) {
        return VolumeToVector::DividePitch(x, y, z);
    }

    bool DivideNumberX(int a) {
        return VolumeToVector::DivideNumberX(a);
    }

    bool DivideNumberY(int a) {
        return VolumeToVector::DivideNumberY(a);
    }

    bool DivideNumberZ(int a) {
        return VolumeToVector::DivideNumberZ(a);
    }

    bool DivideNumber(int x, int y, int z) {
        return VolumeToVector::DivideNumber(x, y, z);
    }

    LUA_SCRIPTCLASS_BEGIN(VolumeToVector_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG3(int,Create,BufferVolumeData_Lua*, double, double)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferLineData_Lua*, LineData)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferTetraData_Lua*, TetraData)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, DividePitch,   double, double, double)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, DivideNumber,  int, int, int)
    LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VolumeToVector_Lua);

#endif //_VOLUMETOVECTOR_LUA_H_

