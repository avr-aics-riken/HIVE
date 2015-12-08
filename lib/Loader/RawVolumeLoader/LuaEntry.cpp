#include "LuaUtil.h"
#include "RawVolumeLoader_Lua.h"

extern "C" {


int luaopen_RawVolumeLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, RawVolumeLoader_Lua);
    SetFunction(L, "RawVolumeLoader",          LUA_SCRIPTCLASS_NEW_FUNCTION(RawVolumeLoader_Lua));
}

}

