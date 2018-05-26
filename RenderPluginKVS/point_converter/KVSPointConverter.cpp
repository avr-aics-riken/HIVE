
#include <kvs/PointObject>
#include <Buffer/Buffer.h>
#include <Buffer/BufferPointData.h>
//#include <SceneScript/BufferPointData_Lua.h>
#include "KVSPointConverter.h"

KVSPointConverter::KVSPointConverter()
{
    kvspoint = new kvs::PointObject();
}

KVSPointConverter::~KVSPointConverter()
{
    delete kvspoint;
}


int KVSPointConverter::setPointBuffer(BufferPointData* pdata)
{
    const Vec3Buffer* pos = pdata->Position();
    const FloatBuffer* radius = pdata->Radius();
    printf("point num = %d\n", pos->GetNum());
    
    //kvspoint->setSize(static_cast<const kvs::Real32>(3.0f));
    kvs::ValueArray<kvs::Real32> sizes;
    const float* pradius = radius->GetBuffer();
    sizes.assign(pradius, radius->GetNum());
    kvspoint->setSizes(sizes);
    
    kvs::ValueArray<kvs::Real32> coords;
    const float* posbuf = pos->GetBuffer();
    coords.assign(posbuf, pos->GetNum()*3);
    kvspoint->setCoords(coords);
    
    return 0;
}

kvs::PointObject* KVSPointConverter::getKVSPointData()
{
    return kvspoint;
}

    
