/**
 * @file SolidModel_Lua.h
 * SolidModel Luaラッパー
 */
#ifndef _SOLIDMODEL_LUA_H_
#define _SOLIDMODEL_LUA_H_


#include "LuaUtil.h"
#include "../RenderObject/SolidModel.h"
#include "BufferSolidData_Lua.h"
#include "RenderObject_Lua.h"
/**
 * SolidModel Luaラッパー
 */
class SolidModel_Lua : public SolidModel
{
public:
    SolidModel_Lua(){}
    ~SolidModel_Lua(){}

    bool Create(BufferSolidData_Lua *tetra) {
        return SolidModel::Create(tetra);
    }

    // RenderObject interface implementation
    IMPLEMENTATION_RENDEROBJECT_LUA()

    LUA_SCRIPTCLASS_BEGIN(SolidModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG0(std::string, GetShader)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, BufferSolidData_Lua*)
    
    // RenderObject interface
    DECLARE_RENDEROBJECT_LUA_METHOD()
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(SolidModel_Lua);


#endif //_SOLIDMODEL_LUA_H_
