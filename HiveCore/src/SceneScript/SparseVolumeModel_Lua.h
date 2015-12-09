/**
 * @file SparseVolumeModel_Lua.h
 * SparseVolumeModel Luaラッパー
 */
#ifndef _SPARSEVOLUMEMODEL_LUA_H_
#define _SPARSEVOLUMEMODEL_LUA_H_

#include "../RenderObject/RenderObject.h"
#include "../RenderObject/SparseVolumeModel.h"
#include "LuaUtil.h"
#include "BufferSparseVolumeData_Lua.h"
#include "RenderObject_Lua.h"
/**
 * SparseVolumeModel Luaラッパー
 */
class SparseVolumeModel_Lua : public SparseVolumeModel
{
public:
    SparseVolumeModel_Lua() {}
    ~SparseVolumeModel_Lua() {}

    bool Create(BufferSparseVolumeData_Lua *vol) {
        SparseVolumeModel::Create(vol);
        return true;
    }

    bool SetClampToEdge(bool s, bool t, bool r) {
        SparseVolumeModel::SetClampToEdge(s, t, r);
        return true;
    }

    // RenderObject interface implementation
    IMPLEMENTATION_RENDEROBJECT_LUA()

    LUA_SCRIPTCLASS_BEGIN(SparseVolumeModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetClampToEdge,bool,bool,bool)
    LUA_SCRIPTCLASS_METHOD_ARG0(std::string, GetShader)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, BufferSparseVolumeData_Lua*)
    
    // RenderObject interface
    DECLARE_RENDEROBJECT_LUA_METHOD()
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(SparseVolumeModel_Lua);

#endif //_SPARSEVOLUMEMODEL_LUA_H_

