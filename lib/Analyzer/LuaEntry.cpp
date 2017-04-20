#include "LuaUtil.h"
#include "Analyzer_Lua.h"

extern "C" {

int luaopen_Analyzer(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, Analyzer_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(Analyzer_Lua));
    return 1;
}

}

