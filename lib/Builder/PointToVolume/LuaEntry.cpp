#include "LuaUtil.h"
#include "BufferMeshData_Lua.h"
#include "PointToVolume_Lua.h"

extern "C" {


int luaopen_PointToVolume(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, PointToVolume_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(PointToVolume_Lua));
    return 1;
}

}

