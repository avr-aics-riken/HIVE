#include "LuaUtil.h"
#include "MandelbulbVolGen_Lua.h"

extern "C" {


int luaopen_MandelbulbVolGen(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, MandelbulbVolGen_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(MandelbulbVolGen_Lua));
    return 1;
}

}

