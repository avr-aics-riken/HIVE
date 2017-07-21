#include "LuaUtil.h"
#include "CpmUtil_Lua.h"

extern "C" {


int luaopen_CdmLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, CPMUtil_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(CPMUtil_Lua));
    return 1;
}

}

