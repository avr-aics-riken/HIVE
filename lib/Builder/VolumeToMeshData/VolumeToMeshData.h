/**
 * @file VolumeToMeshData.h
 * VolumeDataからメッシュ生成を行うモジュール
 */

#ifndef _VOLUMETOMESHDATA_H_
#define _VOLUMETOMESHDATA_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>
#include "BufferVolumeData.h"
#include "BufferMeshData.h"

/**
 * VolumeToMeshData
 */
class VolumeToMeshData : public RefCount
{
private:
    RefPtr<BufferMeshData>    m_mesh;
    double                    m_isovalue;
    RefPtr<BufferVolumeData>  m_volume;   // mutable
public:
    VolumeToMeshData();
    int Create(BufferVolumeData *volume);
    void SetIsoValue(double isovalue);

    // Build isosurface mesh.
    int IsoSurface();

    BufferMeshData* MeshData();
};

#endif //_VOLUMETOMESHDATA_H_

