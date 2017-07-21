/**
 * @file CPMUtil_Lua.h
 * CPMUtil ラッパー
 */
#ifndef HIVE_CPMUTIL_LUA_H_
#define HIVE_CPMUTIL_LUA_H_

#include "CpmUtil.h"
#include "LuaUtil.h"

/**
 * CPMUtil Luaラッパー
 */
class CPMUtil_Lua : public CPMUtil
{
  public:
	CPMUtil_Lua() {}
	~CPMUtil_Lua() {}

	int Proc() { return Proc(); }

	LUA_SCRIPTCLASS_BEGIN(CPMUtil_Lua)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, Proc)
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(CPMUtil_Lua);

#endif // HIVE_CPMUTIL_LUA_H_
