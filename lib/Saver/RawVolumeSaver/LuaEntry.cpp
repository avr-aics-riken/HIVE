#include "LuaUtil.h"
#include "RawVolumeSaver_Lua.h"

extern "C" {


int luaopen_RawVolumeSaver(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, RawVolumeSaver_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(RawVolumeSaver_Lua));
    return 1;
}

}

