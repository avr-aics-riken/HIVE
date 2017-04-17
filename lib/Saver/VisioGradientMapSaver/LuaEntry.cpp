#include "LuaUtil.h"
#include "VisioGradientMapSaver_Lua.h"

extern "C" {


int luaopen_VisioGradientMapSaver(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, VisioGradientMapSaver_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(VisioGradientMapSaver_Lua));
    return 1;
}

}

