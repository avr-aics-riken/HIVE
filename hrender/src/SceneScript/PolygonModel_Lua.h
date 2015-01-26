#ifndef _POLYGONMODEL_LUA_H_
#define _POLYGONMODEL_LUA_H_


#include "../RenderObject/RenderObject.h"
#include "LuaUtil.h"
#include "PolygonModel.h"
#include "BufferMeshData.h"

class PolygonModel_Lua : public PolygonModel
{
public:
	PolygonModel_Lua();
    ~PolygonModel_Lua();
    
    bool SetShader(const std::string& shaderfile);
    bool Create(BufferMeshData mesh);
    
    LUA_SCRIPTCLASS_BEGIN(PolygonModel_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, BufferMeshData)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(PolygonModel_Lua);


#endif //_POLYGONMODEL_LUA_H_
