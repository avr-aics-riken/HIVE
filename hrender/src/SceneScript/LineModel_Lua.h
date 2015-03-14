#ifndef _LINEMODEL_LUA_H_
#define _LINEMODEL_LUA_H_

#include "../RenderObject/RenderObject.h"
#include "LuaUtil.h"
#include "LineModel.h"
#include "BufferLineData_Lua.h"
#include "RenderObject_Lua.h"

class LineModel_Lua : public LineModel
{
public:
    LineModel_Lua() {}
    ~LineModel_Lua() {}

    bool Create(BufferLineData_Lua *mesh) {
        LineModel::Create(mesh);
        return true;
    }

    bool SetLineWidth(float w) {
        return LineModel::SetLineWidth(w);
    }

    // RenderObject interface implementation
    IMPLEMENTATION_RENDEROBJECT_LUA()

    LUA_SCRIPTCLASS_BEGIN(LineModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG0(std::string, GetShader)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetLineWidth, float)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, BufferLineData_Lua*)

    // RenderObject interface
    DECLARE_RENDEROBJECT_LUA_METHOD()
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(LineModel_Lua);


#endif //_LINEMODEL_LUA_H_

