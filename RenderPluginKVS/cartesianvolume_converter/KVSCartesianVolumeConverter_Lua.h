/*
    RenderPluginKVS/KVSCartesianVolumeConverter.h
*/

#pragma once
#include "LuaUtil.h"
#include "KVSCartesianVolumeConverter.h"
#include "BufferVolumeData_Lua.h"

class KVSCartesianVolumeConverter_Lua : public KVSCartesianVolumeConverter
{
private:
    
    KVSCartesianVolumeConverter_Lua()
    : KVSCartesianVolumeConverter()
    {
    }
    
public:
    ~KVSCartesianVolumeConverter_Lua() {}
    
    int setVolumeBuffer(BufferVolumeData_Lua* buf) {
        return KVSCartesianVolumeConverter::setVolumeBuffer(static_cast<BufferVolumeData*>(buf));
    }
    LUA_SCRIPTCLASS_BEGIN(KVSCartesianVolumeConverter_Lua)
    
    LUA_SCRIPTCLASS_METHOD_ARG1(int, setVolumeBuffer, BufferVolumeData_Lua*)
    LUA_SCRIPTCLASS_METHOD_ARG0(void*, getKVSVolumeData)
    
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(KVSCartesianVolumeConverter_Lua);
