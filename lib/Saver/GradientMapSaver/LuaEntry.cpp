#include "LuaUtil.h"
#include "GradientMapSaver_Lua.h"

extern "C" {


int luaopen_GradientMapSaver(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, GradientMapSaver_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(GradientMapSaver_Lua));
    return 1;
}

}

