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

    LUA_SCRIPTCLASS_BEGIN(VolumeModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, BufferVolumeData_Lua*)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VolumeModel_Lua);

#endif //_VOLUMEMODEL_LUA_H_

