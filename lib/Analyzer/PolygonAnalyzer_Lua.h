/**
 * @file PolygonAnalyzer_Lua.h
 * PolygonAnalyzer Luaラッパー
 */
#ifndef _POLYGONANALYZER_LUA_H_
#define _POLYGONANALYZER_LUA_H_

//#include "../RenderObject/RenderObject.h"
#include "LuaUtil.h"
#include "Analyzer.h"
#include "PolygonAnalyzer.h"
#include "BufferMeshData_Lua.h"

/**
 * PolygonAnalyzer Luaラッパー
 */
class PolygonAnalyzer_Lua : public PolygonAnalyzer
{
public:
    PolygonAnalyzer_Lua() {}
    ~PolygonAnalyzer_Lua() {}
    double MinX() {
        return PolygonAnalyzer::MinX();
    }

    double MinY() {
        return PolygonAnalyzer::MinY();
    }

    double MinZ() {
        return PolygonAnalyzer::MinZ();
    }

    double MaxX() {
        return PolygonAnalyzer::MaxX();
    }

    double MaxY() {
        return PolygonAnalyzer::MaxY();
    }

    double MaxZ() {
        return PolygonAnalyzer::MaxZ();
    }

    bool Execute(BufferMeshData_Lua* mesh) {
        return PolygonAnalyzer::Execute(mesh);
    }

    LUA_SCRIPTCLASS_BEGIN(PolygonAnalyzer_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MinX)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MinY)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MinZ)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MaxX)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MaxY)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MaxZ)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("PolygonAnalyzer", bool, Execute, BufferMeshData_Lua*)
    LUA_SCRIPTCLASS_END();

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(PolygonAnalyzer_Lua);


#endif //_POLYGONANALYZER_LUA_H_

