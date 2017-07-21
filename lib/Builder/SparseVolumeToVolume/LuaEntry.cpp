#include "LuaUtil.h"
#include "BufferSparseVolumeData_Lua.h"
#include "SparseVolumeToVolume_Lua.h"

extern "C" {


int luaopen_SparseVolumeToVolume(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, SparseVolumeToVolume_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(SparseVolumeToVolume_Lua));
    return 1;
}

}

