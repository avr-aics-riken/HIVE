#include "LuaUtil.h"
#include "BufferVolumeData_Lua.h"
#include "VolumeFilter_Lua.h"

extern "C" {


int luaopen_VolumeFilter(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, VolumeFilter_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(VolumeFilter_Lua));
    return 1;
}


}
