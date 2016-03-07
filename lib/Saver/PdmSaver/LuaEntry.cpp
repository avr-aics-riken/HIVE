#include "LuaUtil.h"
#include "PdmSaver_Lua.h"

extern "C" {


int luaopen_PdmSaver(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, PDMSaver_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(PDMSaver_Lua));
    return 1;
}

}

