#include "LuaUtil.h"
#include "BufferLineData_Lua.h"
#include "PdbLoader_Lua.h"

extern "C" {


int luaopen_PdbLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, PDBLoader_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(PDBLoader_Lua));
    return 1;
}

}

