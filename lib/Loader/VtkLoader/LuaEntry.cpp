#include "LuaUtil.h"
#include "VtkLoader_Lua.h"

extern "C" {


int luaopen_VtkLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, VTKLoader_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(VTKLoader_Lua));
    return 1;
}

}

