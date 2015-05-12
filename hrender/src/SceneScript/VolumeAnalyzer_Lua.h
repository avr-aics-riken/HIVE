/**
 * @file VolumeAnalyzer_Lua.h
 * VolumeAnalyzer Luaラッパー
 */
#ifndef _VOLUMEANALYZER_LUA_H_
#define _VOLUMEANALYZER_LUA_H_

#include "../RenderObject/RenderObject.h"
#include "LuaUtil.h"
#include "Analyzer.h"
#include "VolumeAnalyzer.h"
#include "VolumeModel_Lua.h"
/**
 * VolumeAnalyzer Luaラッパー
 */
class VolumeAnalyzer_Lua : public VolumeAnalyzer
{
public:
    VolumeAnalyzer_Lua() {}
    ~VolumeAnalyzer_Lua() {}
    double MinX() {
        return VolumeAnalyzer::MinX();
    }

    double MinY() {
        return VolumeAnalyzer::MinY();
    }

    double MinZ() {
        return VolumeAnalyzer::MinZ();
    }

    double MaxX() {
        return VolumeAnalyzer::MaxX();
    }

    double MaxY() {
        return VolumeAnalyzer::MaxY();
    }

    double MaxZ() {
        return VolumeAnalyzer::MaxZ();
    }

    bool Execute(VolumeModel_Lua *model) {
        VolumeAnalyzer::Execute(model);
        return true;
    }

    LUA_SCRIPTCLASS_BEGIN(VolumeAnalyzer_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MinX)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MinY)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MinZ)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MaxX)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MaxY)
    LUA_SCRIPTCLASS_METHOD_ARG0(double, MaxZ)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Execute, VolumeModel_Lua*)
    LUA_SCRIPTCLASS_END();

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VolumeAnalyzer_Lua);


#endif //_VOLUMEANALYZER_LUA_H_

