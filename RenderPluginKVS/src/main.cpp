/*
    RenderPluginKVS/main.cpp
*/

#include "LuaUtil.h"
#include "RenderCoreKVS_Lua.h"
#include <stdio.h>

// LUA export

extern "C" {

int luaopen_RenderPluginKVS(lua_State* L)
{
    printf("CALL luaopen_RenderPluginKVS\n");
    
    LUA_SCRIPTCLASS_REGISTER(L, RenderCoreKVS_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(RenderCoreKVS_Lua));
    
    return 1;
}

}
