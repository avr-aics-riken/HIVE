#ifndef _VOLUMEMODEL_LUA_H_
#define _VOLUMEMODEL_LUA_H_

#include "../RenderObject/RenderObject.h"
#include "LuaUtil.h"
#include "VolumeModel.h"
#include "BufferVolumeData_Lua.h"
#include "RenderObject_Lua.h"

class VolumeModel_Lua : public VolumeModel
{
public:
    VolumeModel_Lua() {}
    ~VolumeModel_Lua() {}

    bool Create(BufferVolumeData_Lua *mesh) {
        VolumeModel::Create(mesh);
        return true;
    }

    // RenderObject interface implementation
    IMPLEMENTATION_RENDEROBJECT_LUA()

    LUA_SCRIPTCLASS_BEGIN(VolumeModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG0(std::string, GetShader)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, BufferVolumeData_Lua*)
    
    // RenderObject interface
    DECLARE_RENDEROBJECT_LUA_METHOD()
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VolumeModel_Lua);

#endif //_VOLUMEMODEL_LUA_H_

