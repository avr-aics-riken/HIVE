#include "LuaUtil.h"
#include "BufferExtraData_Lua.h"
#include "UdmLoader_Lua.h"

extern "C" {

int luaopen_UdmLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, UDMLoader_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(UDMLoader_Lua));
    return 1;
}

}

