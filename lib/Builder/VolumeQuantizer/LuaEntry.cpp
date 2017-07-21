#include "LuaUtil.h"
#include "BufferVolumeData_Lua.h"
#include "VolumeQuantizer_Lua.h"

extern "C" {


int luaopen_VolumeQuantizer(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, VolumeQuantizer_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(VolumeQuantizer_Lua));
    return 1;
}

}

