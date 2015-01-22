#include "../RenderObject/RenderObject.h"
#include "LuaUtil.h"
#include "Loader.h"

class PolygonModel : public RenderObject
{
public:
	PolygonModel();
    ~PolygonModel();
    
    bool SetShader(const std::string& shaderfile);
    bool Create(OBJLoader* obj);
    
    LUA_SCRIPTCLASS_BEGIN(PolygonModel)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, OBJLoader*)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(PolygonModel);

