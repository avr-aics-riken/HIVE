
#include "LuaUtil.h"
#include "BufferSolidData_Lua.h"
#include "BufferExtraData_Lua.h"
#include "SolidDfToVolume_Lua.h"

extern "C" {

int luaopen_SolidDfToVolume(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, SolidDfToVolume_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(SolidDfToVolume_Lua));
    return 1;
}

}

