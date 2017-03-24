#include "LuaUtil.h"
#include "VisioColorMapSaver_Lua.h"

extern "C" {


int luaopen_VisioColorMapSaver(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, VisioColorMapSaver_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(VisioColorMapSaver_Lua));
    return 1;
}

}

