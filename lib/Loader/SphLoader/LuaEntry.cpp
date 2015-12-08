#include "LuaUtil.h"
#include "SphLoader_Lua.h"

extern "C" {


int luaopen_SphLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, SPHLoader_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(SPHLoader_Lua));
    return 1;
}

}

