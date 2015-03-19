/**
 * @file PointModel_Lua.h
 * PointModel Luaラッパー
 */
#ifndef _POINTMODEL_LUA_H_
#define _POINTMODEL_LUA_H_

#include "../RenderObject/RenderObject.h"
#include "LuaUtil.h"
#include "PointModel.h"
#include "BufferPointData_Lua.h"
#include "RenderObject_Lua.h"

class PointModel_Lua : public PointModel
{
public:
    PointModel_Lua() {}
    ~PointModel_Lua() {}

    bool Create(BufferPointData_Lua *mesh) {
        PointModel::Create(mesh);
        return true;
    }

    // RenderObject interface implementation
    IMPLEMENTATION_RENDEROBJECT_LUA()

    LUA_SCRIPTCLASS_BEGIN(PointModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG0(std::string, GetShader)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, BufferPointData_Lua*)
    
    // RenderObject interface
    DECLARE_RENDEROBJECT_LUA_METHOD()
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(PointModel_Lua);


#endif //_POINTMODEL_LUA_H_

