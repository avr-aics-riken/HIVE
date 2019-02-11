/*
    RenderPluginKVS/KVSPointConverter_Lua.h
*/

#pragma once
#include "LuaUtil.h"

namespace kvs {
    class PointObject;
}

class BufferPointData;

class KVSPointConverter : public RefCount
{
private:
    kvs::PointObject* kvspoint;
    
public:
    KVSPointConverter();
    ~KVSPointConverter();
    
    int setPointBuffer(BufferPointData* pdata);
    kvs::PointObject* getKVSPointData();

};
    
