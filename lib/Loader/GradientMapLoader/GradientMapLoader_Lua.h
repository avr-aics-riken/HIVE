/**
 * @file GradientMapLoader_Lua.h
 * GradientMapLoader Luaラッパー
 */
#ifndef _GRADIENTMAPLOADER_LUA_H_
#define _GRADIENTMAPLOADER_LUA_H_

#include "Buffer.h"
#include "LuaUtil.h"
#include "GradientMapLoader.h"
/**
 * GradientMapLoader Luaラッパー
 */
class GradientMapLoader_Lua : public GradientMapLoader
{
  public:
	GradientMapLoader_Lua() {}
	~GradientMapLoader_Lua(){};

	bool Load(const char *filename)
	{
		return GradientMapLoader::Load(filename);
	}

	LUA_SCRIPTCLASS_BEGIN(GradientMapLoader_Lua)
	LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("GradientMapLoader", bool,
											 Load, const char *)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, RGBASize)
	LUA_SCRIPTCLASS_METHOD_ARG1(float, RGBAValue, int)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, GradientSize)
	LUA_SCRIPTCLASS_METHOD_ARG1(float, GradientValue, int)
	LUA_SCRIPTCLASS_METHOD_ARG0(float, MinValue)
	LUA_SCRIPTCLASS_METHOD_ARG0(float, MaxValue)
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(GradientMapLoader_Lua);

#endif //_GRADIENTMAPLOADER_LUA_H_
