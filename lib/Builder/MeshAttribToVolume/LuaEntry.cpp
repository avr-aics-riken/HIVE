
#include "LuaUtil.h"
#include "BufferMeshData_Lua.h"
#include "BufferExtraData_Lua.h"
#include "MeshAttribToVolume_Lua.h"

extern "C" {

int luaopen_MeshAttribToVolume(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, MeshAttribToVolume_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(MeshAttribToVolume_Lua));
    return 1;
}

}

