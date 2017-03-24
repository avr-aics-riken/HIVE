/**
 * @file VisioColorMapLoader_Lua.h
 * VisioColorMapLoader Luaラッパー
 */
#ifndef _VISIOCOLORMAPLOADER_LUA_H_
#define _VISIOCOLORMAPLOADER_LUA_H_


#include "LuaUtil.h"
#include "Buffer.h"
#include "VisioColorMapLoader.h"
/**
 * VisioColorMapLoader Luaラッパー
 */
class VisioColorMapLoader_Lua : public VisioColorMapLoader
{
public:
    VisioColorMapLoader_Lua(){}
    ~VisioColorMapLoader_Lua(){};

    bool Load(const char* filename){
        return VisioColorMapLoader::Load(filename);
    }

    LUA_SCRIPTCLASS_BEGIN(VisioColorMapLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("VisioColorMapLoader",bool,Load,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG0(int, RGBASize)
    LUA_SCRIPTCLASS_METHOD_ARG1(float, RGBAValue, int)
    LUA_SCRIPTCLASS_METHOD_ARG0(float, MinValue)
    LUA_SCRIPTCLASS_METHOD_ARG0(float, MaxValue)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VisioColorMapLoader_Lua);

#endif //_VISIOCOLORMAPLOADER_LUA_H_

