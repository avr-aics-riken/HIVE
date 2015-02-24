#ifndef _VECTORMODEL_LUA_H_
#define _VECTORMODEL_LUA_H_


#include "LuaUtil.h"
#include "../RenderObject/VectorModel.h"
#include "BufferVectorData_Lua.h"

class VectorModel_Lua : public VectorModel
{
public:
    VectorModel_Lua(){}
    ~VectorModel_Lua(){}

    bool SetShader(const std::string& shaderfile) {
        return VectorModel::SetShader(shaderfile);
    }

    bool Create(BufferVectorData_Lua *vec) {
        VectorModel::Create(vec);
        return true;
    }

    bool SetLineWidth(float w) {
        VectorModel::SetLineWidth(w);
        return true;
    }

    bool SetArrowSize(float s) {
        VectorModel::SetArrowSize(s);
        return true;
    }
    bool SetLengthScale(float s) {
        VectorModel::SetLengthScale(s);
        return true;
    }

    
    LUA_SCRIPTCLASS_BEGIN(VectorModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, BufferVectorData_Lua*)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetLineWidth, float)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetArrowSize, float)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetLengthScale, float)

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
LUA_SCRIPTCLASS_CAST_AND_PUSH(VectorModel_Lua);


#endif //_VECTORMODEL_LUA_H_
