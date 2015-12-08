#include "LuaUtil.h"
#include "BufferVolumeData_Lua.h"
#include "BufferVectorData_Lua.h"
#include "VolumeToVector_Lua.h"

extern "C" {


int luaopen_VolumeToVector(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, VolumeToVector_Lua);
    SetFunction(L, "VolumeToVector",          LUA_SCRIPTCLASS_NEW_FUNCTION(VolumeToVector_Lua));
}

}

