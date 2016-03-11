#include "LuaUtil.h"
#include "RawSparseVolumeLoader_Lua.h"

extern "C" {

int luaopen_RawSparseVolumeLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, RawSparseVolumeLoader_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(RawSparseVolumeLoader_Lua));
    return 1;
}

}

