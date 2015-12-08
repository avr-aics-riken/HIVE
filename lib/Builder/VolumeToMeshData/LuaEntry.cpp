#include "LuaUtil.h"
#include "BufferMeshData_Lua.h"
#include "VolumeToMeshData_Lua.h"

extern "C" {


int luaopen_VolumeToMeshData(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, VolumeToMeshData_Lua);
    SetFunction(L, "VolumeToMeshData",          LUA_SCRIPTCLASS_NEW_FUNCTION(VolumeToMeshData_Lua));
}

}

