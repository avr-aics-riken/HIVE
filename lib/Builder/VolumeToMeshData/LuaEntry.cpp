#include "LuaUtil.h"
#include "BufferMeshData_Lua.h"
#include "VolumeToMeshData_Lua.h"

extern "C" {


int luaopen_VolumeToMeshData(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, VolumeToMeshData_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(VolumeToMeshData_Lua));
    return 1;
}

}

