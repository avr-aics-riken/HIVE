#ifndef _CONNECTION_LUA_H_
#define _CONNECTION_LUA_H_

#include "LuaUtil.h"
#include "Connection.h"

class Connection_Lua : public Connection
{
private:
	Connection_Lua(){}
	~Connection_Lua(){}

public:
    LUA_SCRIPTCLASS_BEGIN(Connection_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1 (bool, Connect, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG1 (bool, SendText, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG1 (bool, SendJSON, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG2 (bool, SendBinary, const char*, int)
    LUA_SCRIPTCLASS_METHOD_ARG1 (bool, SendImage, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG0 (std::string, Recv)
    LUA_SCRIPTCLASS_METHOD_ARG0 (bool, Close)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(Connection_Lua);

#endif //_CONNECTION_LUA_H_
