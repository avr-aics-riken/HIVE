#ifndef _POLYGONMODEL_LUA_H_
#define _POLYGONMODEL_LUA_H_


#include "../RenderObject/RenderObject.h"
#include "LuaUtil.h"
#include "PolygonModel.h"
#include "BufferMeshData_Lua.h"

class PolygonModel_Lua : public PolygonModel
{
public:
    PolygonModel_Lua(){}
    ~PolygonModel_Lua(){}

    bool SetShader(const std::string& shaderfile) {
        return PolygonModel::SetShader(shaderfile);
    }

    bool Create(BufferMeshData_Lua *mesh) {
        PolygonModel::Create(mesh);
        return true;
    }

    LUA_SCRIPTCLASS_BEGIN(PolygonModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, BufferMeshData_Lua*)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(PolygonModel_Lua);


#endif //_POLYGONMODEL_LUA_H_
