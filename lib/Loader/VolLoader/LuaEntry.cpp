#include "LuaUtil.h"
#include "VolLoader_Lua.h"

extern "C" {


int luaopen_VolLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, VOLLoader_Lua);
    SetFunction(L, "VOLLoader",          LUA_SCRIPTCLASS_NEW_FUNCTION(VOLLoader_Lua));
}

}

