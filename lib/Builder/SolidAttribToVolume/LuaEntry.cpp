
#include "LuaUtil.h"
#include "BufferSolidData_Lua.h"
#include "BufferExtraData_Lua.h"
#include "SolidAttribToVolume_Lua.h"

extern "C" {

int luaopen_SolidAttribToVolume(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, SolidAttribToVolume_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(SolidAttribToVolume_Lua));
    return 1;
}

}

