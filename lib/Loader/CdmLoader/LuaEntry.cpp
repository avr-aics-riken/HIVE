#include "LuaUtil.h"
#include "CdmLoader_Lua.h"

extern "C" {


int luaopen_CdmLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, CDMLoader_Lua);
    SetFunction(L, "CDMLoader",          LUA_SCRIPTCLASS_NEW_FUNCTION(CDMLoader_Lua));
}

}

