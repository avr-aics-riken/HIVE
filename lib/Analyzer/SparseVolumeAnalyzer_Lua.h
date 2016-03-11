/**
 * @file SparseVolumeAnalyzer_Lua.h
 * SparseVolumeAnalyzer Luaラッパー
 */
#ifndef _SPARSEVOLUMEANALYZER_LUA_H_
#define _SPARSEVOLUMEANALYZER_LUA_H_

#include <vector>
#include "LuaUtil.h"
#include "Analyzer.h"
#include "SparseVolumeAnalyzer.h"
//#include "SparseVolumeModel_Lua.h"
/**
 * SparseVolumeAnalyzer Luaラッパー
 */
class SparseVolumeAnalyzer_Lua : public SparseVolumeAnalyzer
{
public:
    SparseVolumeAnalyzer_Lua() {}
    ~SparseVolumeAnalyzer_Lua() {}
    double MinX() {
        return SparseVolumeAnalyzer::MinX();
    }

    double MinY() {
        return SparseVolumeAnalyzer::MinY();
    }

    double MinZ() {
        return SparseVolumeAnalyzer::MinZ();
    }

    double MaxX() {
        return SparseVolumeAnalyzer::MaxX();
    }

    double MaxY() {
        return SparseVolumeAnalyzer::MaxY();
    }

    double MaxZ() {
        return SparseVolumeAnalyzer::MaxZ();
    }

/*    bool Execute(SparseVolumeModel_Lua *model) {
        SparseVolumeAnalyzer::Execute(model);
        return true;
    }*/
    
    LuaTable GetHistgram() {
        const std::vector<float>& histgram = SparseVolumeAnalyzer::GetHistgram();
        LuaTable t;
        for (int i = 0, size = static_cast<int>(histgram.size()); i < size; ++i) {
            t.push(histgram[i]);
        }
        return t;
    }
    
/*    LuaTable GetHistgramInRange(SparseVolumeModel_Lua *model, double min, double max) {
        const std::vector<float> histgram = SparseVolumeAnalyzer::GetHistgramInRange(model, min, max);
        LuaTable t;
        for (int i = 0, size = static_cast<int>(histgram.size()); i < size; ++i) {
            t.push(histgram[i]);
        }
        return t;
    }*/

    LUA_SCRIPTCLASS_BEGIN(SparseVolumeAnalyzer_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MinX)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MinY)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MinZ)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MaxX)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MaxY)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MaxZ)
//    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Execute, SparseVolumeModel_Lua*) // TODO
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetHistgram);
//    LUA_SCRIPTCLASS_METHOD_ARG3(LuaTable, GetHistgramInRange, SparseVolumeModel_Lua*, double, double); //TODO
    LUA_SCRIPTCLASS_END();

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(SparseVolumeAnalyzer_Lua);


#endif //_SPARSEVOLUMEANALYZER_LUA_H_

