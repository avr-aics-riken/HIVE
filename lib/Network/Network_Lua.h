/**
 * @file Network_Lua.h
 * Network Luaラッパー
 */
#ifndef _NETWORK_LUA_H_
#define _NETWORK_LUA_H_

#include "LuaUtil.h"
#include "Connection_Lua.h"
#include "MetaBinary_Lua.h"

void register_Network_Lua(lua_State* L)
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
    
    lua_setglobal(L, "Network");
}

#endif //_NETWORK_LUA_H_
