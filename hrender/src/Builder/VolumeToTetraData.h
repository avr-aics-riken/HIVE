#ifndef _VOLUMETOTETRADATA_H_
#define _VOLUMETOTETRADATA_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>
#include "BufferTetraData.h"

class VolumeToTetraData : public RefCount
{
private:
    BufferTetraData*  m_tetra;
public:
    VolumeToTetraData();
    int Create(BufferVolumeData *volume, int divide);
    BufferTetraData* TetraData();
};

#endif //_VOLUMETOTETRADATA_H_

