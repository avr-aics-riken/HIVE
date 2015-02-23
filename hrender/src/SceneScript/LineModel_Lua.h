#ifndef _LINEMODEL_LUA_H_
#define _LINEMODEL_LUA_H_

#include "../RenderObject/RenderObject.h"
#include "LuaUtil.h"
#include "LineModel.h"
#include "BufferLineData_Lua.h"

class LineModel_Lua : public LineModel
{
public:
    LineModel_Lua() {}
    ~LineModel_Lua() {}
    bool SetShader(const std::string& shaderfile) {
        return LineModel::SetShader(shaderfile);
    }

    bool Create(BufferLineData_Lua *mesh) {
        LineModel::Create(mesh);
        return true;
    }

    LUA_SCRIPTCLASS_BEGIN(LineModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetLineWidth, float)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, BufferLineData_Lua*)

    // RenderObject interface
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetTranslate,float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetRotate,float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetScale,float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetVec2,  const char*, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG4(bool, SetVec3,  const char*, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG5(bool, SetVec4,  const char*, float, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool, SetFloat, const char*, float)

    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(LineModel_Lua);


#endif //_LINEMODEL_LUA_H_

