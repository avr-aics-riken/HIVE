/**
 * @file ColorMapLoader_Lua.h
 * ColorMapLoader Luaラッパー
 */
#ifndef _COLORMAPLOADER_LUA_H_
#define _COLORMAPLOADER_LUA_H_

#include "Buffer.h"
#include "LuaUtil.h"
#include "ColorMapLoader.h"
/**
 * ColorMapLoader Luaラッパー
 */
class ColorMapLoader_Lua : public ColorMapLoader
{
  public:
	ColorMapLoader_Lua() {}
	~ColorMapLoader_Lua(){};

	bool Load(const char *filename)
	{
		return ColorMapLoader::Load(filename);
	}

	LUA_SCRIPTCLASS_BEGIN(ColorMapLoader_Lua)
	LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("ColorMapLoader", bool, Load,
											 const char *)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, RGBASize)
	LUA_SCRIPTCLASS_METHOD_ARG1(float, RGBAValue, int)
	LUA_SCRIPTCLASS_METHOD_ARG0(float, MinValue)
	LUA_SCRIPTCLASS_METHOD_ARG0(float, MaxValue)
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(ColorMapLoader_Lua);

#endif //_COLORMAPLOADER_LUA_H_
