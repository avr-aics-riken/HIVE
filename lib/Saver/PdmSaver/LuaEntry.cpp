#include "LuaUtil.h"
#include "PdmSaver_Lua.h"

extern "C" {


int luaopen_PdmSaver(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, PDMSaver_Lua);
    SetFunction(L, "PDMSaver",          LUA_SCRIPTCLASS_NEW_FUNCTION(PDMSaver_Lua));
}

}

