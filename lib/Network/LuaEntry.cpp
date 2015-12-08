
#include "Connection_Lua.h"
#include "MetaBinary_Lua.h"

extern "C" {


int luaopen_Connection(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, Connection_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, MetaBinary_Lua);
    SetFunction(L, "Connection",          LUA_SCRIPTCLASS_NEW_FUNCTION(Connection_Lua));
    SetFunction(L, "MetaBinary",          LUA_SCRIPTCLASS_NEW_FUNCTION(MetaBinary_Lua));
}

}

