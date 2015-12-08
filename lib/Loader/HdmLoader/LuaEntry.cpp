#include "LuaUtil.h"
#include "HdmLoader_Lua.h"

extern "C" {


int luaopen_HdmLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, HDMLoader_Lua);
    SetFunction(L, "HDMLoader",          LUA_SCRIPTCLASS_NEW_FUNCTION(HDMLoader_Lua));
}

}

