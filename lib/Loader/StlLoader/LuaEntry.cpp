#include "LuaUtil.h"
#include "StlLoader_Lua.h"

extern "C" {


int luaopen_StlLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, STLLoader_Lua);
    SetFunction(L, "STLLoader",          LUA_SCRIPTCLASS_NEW_FUNCTION(STLLoader_Lua));
}

}

