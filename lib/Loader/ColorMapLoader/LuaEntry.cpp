#include "LuaUtil.h"
#include "ColorMapLoader_Lua.h"

extern "C" {


int luaopen_ColorMapLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, ColorMapLoader_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(ColorMapLoader_Lua));
    return 1;
}

}

