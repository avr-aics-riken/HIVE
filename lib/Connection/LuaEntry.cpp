
#include "Connection_Lua.h"

extern "C" {


int luaopen_Connection(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, Connection_Lua);
    SetFunction(L, "Connection",          LUA_SCRIPTCLASS_NEW_FUNCTION(Connection_Lua));
}

}

