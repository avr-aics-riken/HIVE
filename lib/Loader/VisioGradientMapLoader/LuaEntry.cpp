#include "LuaUtil.h"
#include "VisioGradientMapLoader_Lua.h"

extern "C" {


int luaopen_VisioGradientMapLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, VisioGradientMapLoader_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(VisioGradientMapLoader_Lua));
    return 1;
}

}
