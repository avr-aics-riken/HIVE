/**
 * @file VectorModel_Lua.h
 * VectorModel Luaラッパー
 */
#ifndef _VECTORMODEL_LUA_H_
#define _VECTORMODEL_LUA_H_


#include "LuaUtil.h"
#include "../RenderObject/VectorModel.h"
#include "BufferVectorData_Lua.h"
#include "RenderObject_Lua.h"
/**
 * VectorModel Luaラッパー
 */
class VectorModel_Lua : public VectorModel
{
public:
    VectorModel_Lua(){}
    ~VectorModel_Lua(){}

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

    // RenderObject interface implementation
    IMPLEMENTATION_RENDEROBJECT_LUA()
    
    LUA_SCRIPTCLASS_BEGIN(VectorModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG0(std::string, GetShader)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, BufferVectorData_Lua*)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetLineWidth, float)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetArrowSize, float)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetLengthScale, float)

    // RenderObject interface
    DECLARE_RENDEROBJECT_LUA_METHOD()
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VectorModel_Lua);


#endif //_VECTORMODEL_LUA_H_
