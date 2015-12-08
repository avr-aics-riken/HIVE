/**
 * @file MeshAttribToVolume.h     
 * Mesh attrib データからVolumeDataへ変換するモジュール
 */

#ifndef _MESHATTRIBTOVOLUME_H_
#define _MESHATTRIBTOVOLUME_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferExtraData.h"
#include "BufferVolumeData.h"
#include "BufferMeshData.h"

/**
 * 点 attrib データからVolumeDataへ変換するクラス
 */
class MeshAttribToVolume : public RefCount
{
private:
    BufferMeshData*   m_mesh;
    BufferExtraData*  m_attrib;
    BufferVolumeData* m_volume;

    float             m_bmin[3];
    float             m_bmax[3];

public:
    MeshAttribToVolume();
    ~MeshAttribToVolume();

    /// Assume bounding box computed a priori by the user(e.g. PolygonAnalyzer).
    int Create(BufferMeshData* mesh, BufferExtraData* attrib, float bmin[3], float bmax[3]);

    /// Convert particles to volume. Resultion of volume is set by resX, resY and resZ.
    /// @todo { ROI }
    bool ToVolume(int resX, int resY, int resZ);

    /// Valid after ToVolume()
    BufferVolumeData*  VolumeData();

};

#endif //_MESHATTRIBTOVOLUME_H_

