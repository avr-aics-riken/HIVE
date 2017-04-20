#include "LuaUtil.h"
#include "ClusterParallelCoord_Lua.h"

extern "C" {

int luaopen_ClusterParallelCoord(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, ClusterParallelCoord_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(NetworkLua));
    return 1;
}

}
