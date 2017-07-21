/**
 * @file SolidAnalyzer_Lua.h
 * SolidAnalyzer Luaラッパー
 */
#ifndef _SOLIDANALYZER_LUA_H_
#define _SOLIDANALYZER_LUA_H_

//#include "../RenderObject/RenderObject.h"
#include "LuaUtil.h"
#include "Analyzer.h"
#include "SolidAnalyzer.h"
#include "BufferSolidData_Lua.h"

/**
 * SolidAnalyzer Luaラッパー
 */
class SolidAnalyzer_Lua : public SolidAnalyzer
{
public:
    SolidAnalyzer_Lua() {}
    ~SolidAnalyzer_Lua() {}
    double MinX() {
        return SolidAnalyzer::MinX();
    }

    double MinY() {
        return SolidAnalyzer::MinY();
    }

    double MinZ() {
        return SolidAnalyzer::MinZ();
    }

    double MaxX() {
        return SolidAnalyzer::MaxX();
    }

    double MaxY() {
        return SolidAnalyzer::MaxY();
    }

    double MaxZ() {
        return SolidAnalyzer::MaxZ();
    }

    bool Execute(BufferSolidData_Lua* mesh) {
        return SolidAnalyzer::Execute(mesh);
    }

    LUA_SCRIPTCLASS_BEGIN(SolidAnalyzer_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MinX)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MinY)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MinZ)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MaxX)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MaxY)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MaxZ)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("SolidAnalyzer", bool, Execute, BufferSolidData_Lua*)
    LUA_SCRIPTCLASS_END();

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(SolidAnalyzer_Lua);


#endif //_SOLIDANALYZER_LUA_H_

