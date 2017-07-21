/**
 * @file TPUtil_Lua.h
 * TPUtil ラッパー
 */
#ifndef HIVE_TPUTIL_LUA_H_
#define HIVE_TPUTIL_LUA_H_

#include "TpUtil.h"
#include "LuaUtil.h"

/**
 * TPUtil Luaラッパー
 */
class TPUtil_Lua : public TPUtil
{
  public:
	TPUtil_Lua() {}
	~TPUtil_Lua() {}

	LUA_SCRIPTCLASS_BEGIN(TPUtil_Lua)
	LUA_SCRIPTCLASS_METHOD_ARG1(int, Parse, const char*)
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(TPUtil_Lua);

#endif // HIVE_TPUTIL_LUA_H_
