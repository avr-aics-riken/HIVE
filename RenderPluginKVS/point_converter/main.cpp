/*
    point_converter/main.cpp
*/

#include "LuaUtil.h"
#include "KVSPointConverter_Lua.h"
#include <stdio.h>

// LUA export

extern "C" {

int luaopen_KVSPointConverter(lua_State* L)
{
    printf("CALL luaopen_KVSPointConverter\n");
    
    LUA_SCRIPTCLASS_REGISTER(L, KVSPointConverter_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(KVSPointConverter_Lua));
    
    return 1;
}

}
