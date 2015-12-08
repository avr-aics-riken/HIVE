#include "LuaUtil.h"
#include "BufferLineData_Lua.h"
#include "PdbLoader_Lua.h"

extern "C" {


int luaopen_PdbLoader(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, PDBLoader_Lua);
    SetFunction(L, "PDBLoader",          LUA_SCRIPTCLASS_NEW_FUNCTION(PDBLoader_Lua));
}

}

