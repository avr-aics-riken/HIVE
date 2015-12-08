#include "LuaUtil.h"
#include "ObjLoader_Lua.h"

extern "C" {


int luaopen_ObjLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, OBJLoader_Lua);
    SetFunction(L, "OBJLoader",          LUA_SCRIPTCLASS_NEW_FUNCTION(OBJLoader_Lua));
}

}

