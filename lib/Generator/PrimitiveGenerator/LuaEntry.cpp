#include "LuaUtil.h"
#include "PrimitiveGenerator_Lua.h"

extern "C" {


int luaopen_PrimitiveGenerator(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, PrimitiveGenerator_Lua);
    SetFunction(L, "PrimitiveGenerator",          LUA_SCRIPTCLASS_NEW_FUNCTION(PrimitiveGenerator_Lua));
}

}

