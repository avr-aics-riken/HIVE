#ifndef _VOLUMETOMESHDATA_H_
#define _VOLUMETOMESHDATA_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>
#include "BufferVolumeData.h"
#include "BufferMeshData.h"

class VolumeToMeshData : public RefCount
{
private:
    BufferMeshData*  m_mesh;
public:
    VolumeToMeshData();
    int Create(BufferVolumeData *volume, double isovalue);
    BufferMeshData* MeshData();
};

#endif //_VOLUMETOMESHDATA_H_

