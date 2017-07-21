#include "LuaUtil.h"
#include "ColorMapSaver_Lua.h"

extern "C" {


int luaopen_ColorMapSaver(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, ColorMapSaver_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(ColorMapSaver_Lua));
    return 1;
}

}

