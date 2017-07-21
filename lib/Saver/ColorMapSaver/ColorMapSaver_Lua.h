/**
 * @file ColorMapSaver_Lua.h
 * ColorMapSaver Luaラッパー
 */
#ifndef _COLORMAPSAVER_LUA_H_
#define _COLORMAPSAVER_LUA_H_


#include "LuaUtil.h"
#include "Buffer.h"
#include "ColorMapSaver.h"
/**
 * ColorMapSaver Luaラッパー
 */
class ColorMapSaver_Lua : public ColorMapSaver
{
public:
    ColorMapSaver_Lua(){}
    ~ColorMapSaver_Lua(){};

    bool Save(const char* filename){
        return ColorMapSaver::Save(filename);
    }

    LUA_SCRIPTCLASS_BEGIN(ColorMapSaver_Lua)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("ColorMapSaver",bool,Save,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG0(int, RGBASize)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool, SetRGBAValue, int, float)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetMinValue, float)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetMaxValue, float)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(ColorMapSaver_Lua);

#endif //_COLORMAPSAVER_LUA_H_

