#include "LuaUtil.h"
#include "HdmLoader_Lua.h"

extern "C" {


int luaopen_HdmLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, HDMLoader_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(HDMLoader_Lua));
    return 1;
}

}

