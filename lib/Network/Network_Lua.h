/**
 * @file Network_Lua.h
 * Network Luaラッパー
 */
#ifndef _NETWORK_LUA_H_
#define _NETWORK_LUA_H_

#include "LuaUtil.h"
#include "Connection_Lua.h"
#include "MetaBinary_Lua.h"

/**
 * Network Luaラッパー
 */
class Network_Lua  : public RefCount
{
private:
	Network_Lua(){}
	~Network_Lua(){}
public:

    Connection_Lua* Connection() {
        return Connection_Lua::CreateInstance();
    }
    MetaBinary_Lua* MetaBinary() {
        return MetaBinary_Lua::CreateInstance();
    }

    LUA_SCRIPTCLASS_BEGIN(Network_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0 (Connection_Lua*, Connection)
    LUA_SCRIPTCLASS_METHOD_ARG0 (MetaBinary_Lua*, MetaBinary)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(Network_Lua);

#endif //_NETWORK_LUA_H_
