#include "LuaUtil.h"
#include "SphSaver_Lua.h"

extern "C" {


int luaopen_SphSaver(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, SPHSaver_Lua);
    SetFunction(L, "SPHSaver",          LUA_SCRIPTCLASS_NEW_FUNCTION(SPHSaver_Lua));
}

}

