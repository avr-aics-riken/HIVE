#include "LuaUtil.h"
#include "BcmLoader_Lua.h"

extern "C" {


int luaopen_BcmLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, BCMLoader_Lua);
    SetFunction(L, "BCMLoader",          LUA_SCRIPTCLASS_NEW_FUNCTION(BCMLoader_Lua));
}
}
