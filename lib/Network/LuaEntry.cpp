#include "LuaUtil.h"
#include "NetworkLua.h"

extern "C" {


int luaopen_Network(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, NetworkLua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(NetworkLua));
    return 1;
}

}
