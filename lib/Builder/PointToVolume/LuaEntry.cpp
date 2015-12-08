#include "LuaUtil.h"
#include "BufferMeshData_Lua.h"
#include "PointToVolume_Lua.h"

extern "C" {


int luaopen_PointToVolume(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, PointToVolume_Lua);
    SetFunction(L, "PointToVolume",          LUA_SCRIPTCLASS_NEW_FUNCTION(PointToVolume_Lua));
}

}

