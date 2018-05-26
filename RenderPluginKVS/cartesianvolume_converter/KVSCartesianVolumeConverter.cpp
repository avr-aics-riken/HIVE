
#include <kvs/StructuredVolumeObject>
#include <Buffer/Buffer.h>
#include <Buffer/BufferVolumeData.h>
#include "KVSCartesianVolumeConverter.h"

KVSCartesianVolumeConverter::KVSCartesianVolumeConverter()
{
    kvsvolume = 0;
}

KVSCartesianVolumeConverter::~KVSCartesianVolumeConverter()
{
    delete kvsvolume;
}


int KVSCartesianVolumeConverter::setPointBuffer(BufferVolumeData* vdata)
{
    const int w = vdata->Width();
    const int h = vdata->Height();
    const int d = vdata->Depth();
    const int c = vdata->Component();
    
    const int sizeOfFloat = 4;
    kvs::ValueArray<kvs::Real32> values( vdata->Pointer(), w * h * d * c * sizeOfFloat );

    delete kvsvolume;
    kvsvolume = new kvs::StructuredVolumeObject();
    kvsvolume->setGridType( kvs::StructuredVolumeObject::Uniform );
    const size_t veclen = vdata->Component();
    kvsvolume->setVeclen( veclen );
    kvsvolume->setResolution( kvs::Vector3ui( w, h, d ) );
    kvsvolume->setValues( kvs::AnyValueArray( values ) );
    
    return 0;
}

kvs::StructuredVolumeObject* KVSCartesianVolumeConverter::getKVSVolumeData()
{
    return kvsvolume;
}

    
