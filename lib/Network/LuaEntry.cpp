#include "LuaUtil.h"
#include "Network_Lua.h"

extern "C" {


int luaopen_Network(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, Network_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(Network_Lua));
    return 1;
}

}
