/**
 * @file PolygonModel_Lua.h
 * PolygonModel Luaラッパー
 */
#ifndef _POLYGONMODEL_LUA_H_
#define _POLYGONMODEL_LUA_H_


#include "../RenderObject/RenderObject.h"
#include "LuaUtil.h"
#include "PolygonModel.h"
#include "BufferMeshData_Lua.h"
#include "RenderObject_Lua.h"
/**
 * PolygonModel Luaラッパー
 */
class PolygonModel_Lua : public PolygonModel
{
public:
    PolygonModel_Lua(){}
    ~PolygonModel_Lua(){}

    bool Create(BufferMeshData_Lua *mesh) {
        PolygonModel::Create(mesh);
        return true;
    }
    
    // RenderObject interface implementation
    IMPLEMENTATION_RENDEROBJECT_LUA()
    
    LUA_SCRIPTCLASS_BEGIN(PolygonModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG0(std::string, GetShader)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, BufferMeshData_Lua*)
    
    // RenderObject interface
    DECLARE_RENDEROBJECT_LUA_METHOD()
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(PolygonModel_Lua);


#endif //_POLYGONMODEL_LUA_H_
