#ifndef _TETRAMODEL_LUA_H_
#define _TETRAMODEL_LUA_H_


#include "LuaUtil.h"
#include "../RenderObject/TetraModel.h"
#include "BufferVectorData_Lua.h"

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

    LUA_SCRIPTCLASS_BEGIN(TetraModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, TetraModel*)
    
    // RenderObject interface
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetTranslate, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetRotate,    float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetScale,     float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetVec2,      const char*, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG4(bool, SetVec3,      const char*, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG5(bool, SetVec4,      const char*, float, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool, SetFloat,     const char*, float)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(TetraModel_Lua);


#endif //_TETRAMODEL_LUA_H_
