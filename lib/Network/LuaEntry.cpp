
#include "Connection_Lua.h"
#include "MetaBinary_Lua.h"

extern "C" {


int luaopen_Network(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, Connection_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, MetaBinary_Lua);
    lua_newtable(L);
    lua_pushstring(L, "Connection"); 
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(Connection_Lua));
    lua_settable(L, -3);

    lua_pushstring(L, "MetaBinary"); 
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(MetaBinary_Lua));
    lua_settable(L, -3);
    
    return 1;
}

}
