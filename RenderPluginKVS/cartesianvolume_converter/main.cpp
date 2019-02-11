/*
    cartesianvolume_converter/main.cpp
*/

#include "LuaUtil.h"
#include "KVSCartesianVolumeConverter_Lua.h"
#include <stdio.h>

// LUA export

extern "C" {

int luaopen_KVSCartesianVolumeConverter(lua_State* L)
{
    printf("CALL luaopen_KVSCartesianVolumeConverter\n");
    
    LUA_SCRIPTCLASS_REGISTER(L, KVSCartesianVolumeConverter_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(KVSCartesianVolumeConverter_Lua));
    
    return 1;
}

}
