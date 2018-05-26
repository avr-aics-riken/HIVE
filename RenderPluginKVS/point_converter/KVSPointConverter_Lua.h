/*
    RenderPluginKVS/KVSPointConverter_Lua.h
*/

#pragma once
#include "LuaUtil.h"
#include "KVSPointConverter.h"
#include "BufferPointData_Lua.h"

class KVSPointConverter_Lua : public KVSPointConverter
{
private:
    
    KVSPointConverter_Lua()
    : KVSPointConverter()
    {
    }
    
public:
    ~KVSPointConverter_Lua() {}
    
    int setPointBuffer(BufferPointData_Lua* buf) {
        return KVSPointConverter::setPointBuffer(static_cast<BufferPointData*>(buf));
    }
    LUA_SCRIPTCLASS_BEGIN(KVSPointConverter_Lua)
    
    LUA_SCRIPTCLASS_METHOD_ARG1(int, setPointBuffer, BufferPointData_Lua*)
    LUA_SCRIPTCLASS_METHOD_ARG0(void*, getKVSPointData)
    /*LUA_SCRIPTCLASS_METHOD_ARG2(int, render, LuaTable, LuaTable);
    LUA_SCRIPTCLASS_METHOD_ARG0(int, clearCache);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, clearShaderCache, const char*);
    LUA_SCRIPTCLASS_METHOD_ARG0(std::string, getRendererName);
*/
    
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(KVSPointConverter_Lua);
