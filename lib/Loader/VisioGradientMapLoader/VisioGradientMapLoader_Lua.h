/**
 * @file VisioGradientMapLoader_Lua.h
 * VisioGradientMapLoader Luaラッパー
 */
#ifndef _VISIOGRADIENTMAPLOADER_LUA_H_
#define _VISIOGRADIENTMAPLOADER_LUA_H_

#include "Buffer.h"
#include "LuaUtil.h"
#include "VisioGradientMapLoader.h"
/**
 * VisioGradientMapLoader Luaラッパー
 */
class VisioGradientMapLoader_Lua : public VisioGradientMapLoader
{
  public:
	VisioGradientMapLoader_Lua() {}
	~VisioGradientMapLoader_Lua(){};

	bool Load(const char *filename)
	{
		return VisioGradientMapLoader::Load(filename);
	}

	LUA_SCRIPTCLASS_BEGIN(VisioGradientMapLoader_Lua)
	LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("VisioGradientMapLoader", bool,
											 Load, const char *)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, RGBASize)
	LUA_SCRIPTCLASS_METHOD_ARG1(float, RGBAValue, int)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, GradientSize)
	LUA_SCRIPTCLASS_METHOD_ARG1(float, GradientValue, int)
	LUA_SCRIPTCLASS_METHOD_ARG0(float, MinValue)
	LUA_SCRIPTCLASS_METHOD_ARG0(float, MaxValue)
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VisioGradientMapLoader_Lua);

#endif //_VISIOGRADIENTMAPLOADER_LUA_H_
