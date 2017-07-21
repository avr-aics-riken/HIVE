#include "LuaUtil.h"
#include "BufferImageData_Lua.h"
#include "ImageFilter_Lua.h"

extern "C" {


int luaopen_ImageFilter(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, ImageFilter_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(ImageFilter_Lua));
    return 1;
}


}
