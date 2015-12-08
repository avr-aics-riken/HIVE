#include "LuaUtil.h"
#include "VolumeAnalyzer_Lua.h"
#include "PolygonAnalyzer_Lua.h"

extern "C" {

int luaopen_Analyzer(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, VolumeAnalyzer_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, PolygonAnalyzer_Lua);
    SetFunction(L, "VolumeAnalyzer",          LUA_SCRIPTCLASS_NEW_FUNCTION(VolumeAnalyzer_Lua));
    SetFunction(L, "PolygonAnalyzer",          LUA_SCRIPTCLASS_NEW_FUNCTION(PolygonAnalyzer_Lua));
}

}

