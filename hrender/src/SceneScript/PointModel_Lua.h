#ifndef _POINTMODEL_LUA_H_
#define _POINTMODEL_LUA_H_

#include "../RenderObject/RenderObject.h"
#include "LuaUtil.h"
#include "PointModel.h"
#include "BufferPointData_Lua.h"

class PointModel_Lua : public PointModel
{
public:
    PointModel_Lua() {}
    ~PointModel_Lua() {}
    bool SetShader(const std::string& shaderfile) {
        return PointModel::SetShader(shaderfile);
    }

    bool Create(BufferPointData_Lua *mesh) {
        PointModel::Create(mesh);
        return true;
    }

    LUA_SCRIPTCLASS_BEGIN(PointModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, BufferPointData_Lua*)
    
    // RenderObject interface
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetTranslate, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetRotate,float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetScale,float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetVec2,  const char*, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG4(bool, SetVec3,  const char*, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG5(bool, SetVec4,  const char*, float, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool, SetFloat, const char*, float)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(PointModel_Lua);


#endif //_POINTMODEL_LUA_H_

