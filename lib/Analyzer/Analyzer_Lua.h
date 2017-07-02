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
void register_Analyzer_Lua(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, VolumeAnalyzer_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, SparseVolumeAnalyzer_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, PolygonAnalyzer_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, SolidAnalyzer_Lua);
    lua_newtable(L);
    lua_pushstring(L, "VolumeAnalyzer");
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(VolumeAnalyzer_Lua));
    lua_settable(L, -3);

    lua_pushstring(L, "SparseVolumeAnalyzer");
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(SparseVolumeAnalyzer_Lua));
    lua_settable(L, -3);

    lua_pushstring(L, "PolygonAnalyzer");
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(PolygonAnalyzer_Lua));
    lua_settable(L, -3);

    lua_pushstring(L, "SolidAnalyzer");
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(SolidAnalyzer_Lua));
    lua_settable(L, -3);
    
    lua_setglobal(L, "Analyzer");
};

#endif //_ANALYZER_LUA_H_
