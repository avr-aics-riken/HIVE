#include "LuaUtil.h"
#include "TpUtil_Lua.h"

extern "C" {


int luaopen_TpUtil(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, TPUtil_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(TPUtil_Lua));
    return 1;
}

}

