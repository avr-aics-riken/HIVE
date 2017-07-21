/**
 * @file GradientMapSaver_Lua.h
 * GradientMapSaver Luaラッパー
 */
#ifndef _GRADIENTMAPSAVER_LUA_H_
#define _GRADIENTMAPSAVER_LUA_H_


#include "LuaUtil.h"
#include "Buffer.h"
#include "GradientMapSaver.h"
/**
 * GradientMapSaver Luaラッパー
 */
class GradientMapSaver_Lua : public GradientMapSaver
{
public:
    GradientMapSaver_Lua(){}
    ~GradientMapSaver_Lua(){};

    bool Save(const char* filename){
        return GradientMapSaver::Save(filename);
    }

    LUA_SCRIPTCLASS_BEGIN(GradientMapSaver_Lua)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("GradientMapSaver",bool,Save,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG0(int, RGBASize)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool, SetRGBAValue, int, float)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetMinValue, float)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetMaxValue, float)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(GradientMapSaver_Lua);

#endif //_GRADIENTMAPSAVER_LUA_H_

