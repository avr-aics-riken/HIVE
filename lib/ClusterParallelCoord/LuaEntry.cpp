#include "LuaUtil.h"
#include "VolumeClusterAnalyzer_Lua.h"

extern "C" {

int luaopen_VolumeClusterAnalyzer(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, VolumeClusterAnalyzer_Lua);
    lua_newtable(L);
    lua_pushstring(L, "VolumeClusterAnalyzer");
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(VolumeClusterAnalyzer_Lua));
    lua_settable(L, -3);

    return 1;
}

}

