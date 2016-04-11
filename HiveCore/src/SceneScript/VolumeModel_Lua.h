/**
 * @file VolumeModel_Lua.h
 * VolumeModel Luaラッパー
 */
#ifndef _VOLUMEMODEL_LUA_H_
#define _VOLUMEMODEL_LUA_H_

#include "../RenderObject/RenderObject.h"
#include "../RenderObject/VolumeModel.h"
#include "LuaUtil.h"
#include "BufferVolumeData_Lua.h"
#include "RenderObject_Lua.h"
/**
 * VolumeModel Luaラッパー
 */
class VolumeModel_Lua : public VolumeModel
{
public:
    VolumeModel_Lua() {}
    ~VolumeModel_Lua() {}

    bool Create(BufferVolumeData_Lua *vol) {
        VolumeModel::Create(vol);
        return true;
    }

    bool SetClampToEdge(bool s, bool t, bool r) {
        VolumeModel::SetClampToEdge(s, t, r);
        return true;
    }
    
    BufferVolumeData_Lua* GetVolume() {
        return new BufferVolumeData_Lua(VolumeModel::GetVolume());
    }

    // RenderObject interface implementation
    IMPLEMENTATION_RENDEROBJECT_LUA()

    LUA_SCRIPTCLASS_BEGIN(VolumeModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetClampToEdge,bool,bool,bool)
    LUA_SCRIPTCLASS_METHOD_ARG0(std::string, GetShader)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua, GetVolume)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, BufferVolumeData_Lua*)
    
    // RenderObject interface
    DECLARE_RENDEROBJECT_LUA_METHOD()
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VolumeModel_Lua);

#endif //_VOLUMEMODEL_LUA_H_

