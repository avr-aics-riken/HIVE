#include "LuaUtil.h"
#include "Volren_Lua.h"

extern "C" {


int luaopen_Volren(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, Volren_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(Volren_Lua));
    return 1;
}

}

