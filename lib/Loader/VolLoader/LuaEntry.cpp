#include "LuaUtil.h"
#include "VolLoader_Lua.h"

extern "C" {


int luaopen_VolLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, VOLLoader_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(VOLLoader_Lua));
    return 1;
}

}

