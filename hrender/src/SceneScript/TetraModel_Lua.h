#ifndef _TETRAMODEL_LUA_H_
#define _TETRAMODEL_LUA_H_


#include "LuaUtil.h"
#include "../RenderObject/TetraModel.h"
#include "BufferVectorData_Lua.h"
#include "RenderObject_Lua.h"

class TetraModel_Lua : public TetraModel
{
public:
    TetraModel_Lua(){}
    ~TetraModel_Lua(){}

    bool SetShader(const std::string& shaderfile) {
        return TetraModel::SetShader(shaderfile);
    }

    bool Create(TetraModel *tetra) {
        TetraModel_Lua::Create(tetra);
        return true;
    }

    // RenderObject interface implementation
    IMPLEMENTATION_RENDEROBJECT_LUA()

    LUA_SCRIPTCLASS_BEGIN(TetraModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, TetraModel*)
    
    // RenderObject interface
    DECLARE_RENDEROBJECT_LUA_METHOD()
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(TetraModel_Lua);


#endif //_TETRAMODEL_LUA_H_
