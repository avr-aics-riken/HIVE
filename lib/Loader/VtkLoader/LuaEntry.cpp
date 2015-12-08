#include "LuaUtil.h"
#include "VtkLoader_Lua.h"

extern "C" {


int luaopen_VtkLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, VTKLoader_Lua);
    SetFunction(L, "VTKLoader",          LUA_SCRIPTCLASS_NEW_FUNCTION(VTKLoader_Lua));
}

}

