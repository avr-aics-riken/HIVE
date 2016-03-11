/**
 * @file Connection_Lua.h
 * Connection Luaラッパー
 */
#ifndef _CONNECTION_LUA_H_
#define _CONNECTION_LUA_H_

#include "LuaUtil.h"
#include "Connection.h"
/**
 * Connection Luaラッパー
 */
class Connection_Lua : public Connection
{
private:
	Connection_Lua(){}
	~Connection_Lua(){}

public:
    LUA_SCRIPTCLASS_BEGIN(Connection_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1 (bool, Connect, const std::string&)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1 ("Connection", bool, SendText, const std::string&)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1 ("Connection", bool, SendJSON, const std::string&)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG2 ("Connection", bool, SendBinary, const Connection::Binary, int)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1 ("Connection", bool, SendImage, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG0 (std::string, Recv)
    LUA_SCRIPTCLASS_METHOD_ARG0 (std::string, GetState)
    LUA_SCRIPTCLASS_METHOD_ARG1 (bool, SetTimeout, int)
    LUA_SCRIPTCLASS_METHOD_ARG0 (bool, Close)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(Connection_Lua);

#endif //_CONNECTION_LUA_H_
