#ifndef _VOLUMEMODEL_LUA_H_
#define _VOLUMEMODEL_LUA_H_

#include "../RenderObject/RenderObject.h"
#include "LuaUtil.h"
#include "VolumeModel.h"
#include "BufferVolumeData_Lua.h"

class VolumeModel_Lua : public VolumeModel
{
public:
    VolumeModel_Lua() {}
    ~VolumeModel_Lua() {}
    bool SetShader(const std::string& shaderfile) {
        return VolumeModel::SetShader(shaderfile);
    }

    bool Create(BufferVolumeData_Lua *mesh) {
        VolumeModel::Create(mesh);
        return true;
    }

    bool SetTranslate(float x, float y, float z) {
        RenderObject::SetTranslate(x, y, z);
        return true;
    }

    bool SetRotate(float x, float y, float z) {
        RenderObject::SetRotate(x, y, z);
        return true;
    }

    bool SetScale(float x, float y, float z) {
        RenderObject::SetScale(x, y, z);
        return true;
    }

    bool SetVec4(const char* name, float x, float y, float z, float w) {
        RenderObject::SetVec4(std::string(name), x, y, z, w);
        return true;
    }

    bool SetVec3(const char* name, float x, float y, float z) {
        RenderObject::SetVec3(std::string(name), x, y, z);
        return true;
    }

    bool SetVec2(const char* name, float x, float y) {
        RenderObject::SetVec2(std::string(name), x, y);
        return true;
    }

    bool SetFloat(const char* name, float x) {
        RenderObject::SetFloat(std::string(name), x);
        return true;
    }

    LUA_SCRIPTCLASS_BEGIN(VolumeModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, BufferVolumeData_Lua*)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetTranslate,float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetRotate,float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetScale,float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetVec2,  const char*, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG4(bool, SetVec3,  const char*, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG5(bool, SetVec4,  const char*, float, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool, SetFloat, const char*, float)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VolumeModel_Lua);

#endif //_VOLUMEMODEL_LUA_H_

