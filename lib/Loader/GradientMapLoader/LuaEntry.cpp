#include "LuaUtil.h"
#include "GradientMapLoader_Lua.h"

extern "C" {


int luaopen_GradientMapLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, GradientMapLoader_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(GradientMapLoader_Lua));
    return 1;
}

}
