/**
 * @file Analyzer_Lua.h
 * Analyzer Luaラッパー
 */
#ifndef _ANALYZER_LUA_H_
#define _ANALYZER_LUA_H_

#include "LuaUtil.h"
#include "VolumeAnalyzer_Lua.h"
#include "SparseVolumeAnalyzer_Lua.h"
#include "PolygonAnalyzer_Lua.h"
#include "SolidAnalyzer_Lua.h"

/**
 * Analyzer Luaラッパー
 */
class Analyzer_Lua  : public RefCount
{
private:
	Analyzer_Lua(){}
	~Analyzer_Lua(){}
public:

    VolumeAnalyzer_Lua* VolumeAnalyzer() {
        return new VolumeAnalyzer_Lua();
    }
    SparseVolumeAnalyzer_Lua* SparseVolumeAnalyzer() {
        return new SparseVolumeAnalyzer_Lua();
    }
    PolygonAnalyzer_Lua* PolygonAnalyzer() {
        return new PolygonAnalyzer_Lua();
    }
    SolidAnalyzer_Lua* SolidAnalyzer() {
        return new SolidAnalyzer_Lua();
    }

    LUA_SCRIPTCLASS_BEGIN(Analyzer_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0 (VolumeAnalyzer_Lua*, VolumeAnalyzer)
    LUA_SCRIPTCLASS_METHOD_ARG0 (SparseVolumeAnalyzer_Lua*, SparseVolumeAnalyzer)
    LUA_SCRIPTCLASS_METHOD_ARG0 (PolygonAnalyzer_Lua*, PolygonAnalyzer)
    LUA_SCRIPTCLASS_METHOD_ARG0 (SolidAnalyzer_Lua*, SolidAnalyzer)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(Analyzer_Lua);

#endif //_ANALYZER_LUA_H_
