#include "LuaUtil.h"
#include "BufferMeshData_Lua.h"
#include "FloatsToFloat_Lua.h"

extern "C" {


int luaopen_FloatsToFloat(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, FloatsToFloat_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(FloatsToFloat_Lua));
    return 1;
}

}

