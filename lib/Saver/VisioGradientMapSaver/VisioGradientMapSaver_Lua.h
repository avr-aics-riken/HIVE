/**
 * @file VisioGradientMapSaver_Lua.h
 * VisioGradientMapSaver Luaラッパー
 */
#ifndef _VISIOGRADIENTMAPSAVER_LUA_H_
#define _VISIOGRADIENTMAPSAVER_LUA_H_


#include "LuaUtil.h"
#include "Buffer.h"
#include "VisioGradientMapSaver.h"
/**
 * VisioGradientMapSaver Luaラッパー
 */
class VisioGradientMapSaver_Lua : public VisioGradientMapSaver
{
public:
    VisioGradientMapSaver_Lua(){}
    ~VisioGradientMapSaver_Lua(){};

    bool Save(const char* filename){
        return VisioGradientMapSaver::Save(filename);
    }

    LUA_SCRIPTCLASS_BEGIN(VisioGradientMapSaver_Lua)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("VisioGradientMapSaver",bool,Save,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG0(int, RGBASize)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool, SetRGBAValue, int, float)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetMinValue, float)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetMaxValue, float)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VisioGradientMapSaver_Lua);

#endif //_VISIOGRADIENTMAPSAVER_LUA_H_

