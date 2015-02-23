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

    bool DividePitchX(double a) { VolumeToVector::DividePitchX(a);  return  true; }
    bool DividePitchY(double a) { VolumeToVector::DividePitchY(a);  return  true; }
    bool DividePitchZ(double a) { VolumeToVector::DividePitchZ(a);  return  true; }
    bool DivideNumberX(double a){ VolumeToVector::DivideNumberX(a); return  true; }
    bool DivideNumberY(double a){ VolumeToVector::DivideNumberY(a); return  true; }
    bool DivideNumberZ(double a){ VolumeToVector::DivideNumberZ(a); return  true; }

    LUA_SCRIPTCLASS_BEGIN(VolumeToVector_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG3(int,Create,BufferVolumeData_Lua*, double, double)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferLineData_Lua*, LineData)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferTetraData_Lua*, TetraData)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, DividePitchX , double)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, DividePitchY , double)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, DividePitchZ , double)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, DivideNumberX, double)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, DivideNumberY, double)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, DivideNumberZ, double)
    LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VolumeToVector_Lua);

#endif //_VOLUMETOVECTOR_LUA_H_

