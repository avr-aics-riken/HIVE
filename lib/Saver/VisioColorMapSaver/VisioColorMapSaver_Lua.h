/**
 * @file VisioColorMapSaver_Lua.h
 * VisioColorMapSaver Luaラッパー
 */
#ifndef _VISIOCOLORMAPSAVER_LUA_H_
#define _VISIOCOLORMAPSAVER_LUA_H_


#include "LuaUtil.h"
#include "Buffer.h"
#include "VisioColorMapSaver.h"
/**
 * VisioColorMapSaver Luaラッパー
 */
class VisioColorMapSaver_Lua : public VisioColorMapSaver
{
public:
    VisioColorMapSaver_Lua(){}
    ~VisioColorMapSaver_Lua(){};

    bool Save(const char* filename){
        return VisioColorMapSaver::Save(filename);
    }

    LUA_SCRIPTCLASS_BEGIN(VisioColorMapSaver_Lua)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("VisioColorMapSaver",bool,Save,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG0(int, RGBASize)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool, SetRGBAValue, int, float)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetMinValue, float)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetMaxValue, float)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VisioColorMapSaver_Lua);

#endif //_VISIOCOLORMAPSAVER_LUA_H_

