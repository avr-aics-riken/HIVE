#include "LuaUtil.h"
#include "VisioColorMapLoader_Lua.h"

extern "C" {


int luaopen_VisioColorMapLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, VisioColorMapLoader_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(VisioColorMapLoader_Lua));
    return 1;
}

}

