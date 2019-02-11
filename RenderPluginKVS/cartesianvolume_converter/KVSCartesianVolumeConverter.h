/*
    RenderPluginKVS/KVSCartesianVolumeConverter.h
*/

#pragma once
#include "LuaUtil.h"

namespace kvs {
    class StructuredVolumeObject;
}

class BufferVolumeData;

class KVSCartesianVolumeConverter : public RefCount
{
private:
    kvs::StructuredVolumeObject* kvsvolume;
    
public:
    KVSCartesianVolumeConverter();
    ~KVSCartesianVolumeConverter();
    
    int setVolumeBuffer(BufferVolumeData* pdata);
    kvs::StructuredVolumeObject* getKVSVolumeData();

};
    
