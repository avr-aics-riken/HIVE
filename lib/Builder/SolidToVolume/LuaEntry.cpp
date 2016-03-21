#include "LuaUtil.h"
#include "BufferSolidData_Lua.h"
#include "BufferExtraData_Lua.h"
#include "SolidToVolume_Lua.h"

extern "C" {

int luaopen_SolidToVolume(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, SolidToVolume_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(SolidToVolume_Lua));
    return 1;
}

}

