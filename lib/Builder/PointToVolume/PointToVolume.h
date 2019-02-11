/**
 * @file PointToVolume.h     
 * 点データからVolumeDataへ変換するモジュール
 */

#ifndef _POINTTOVOLUME_H_
#define _POINTTOVOLUME_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferPointData.h"
#include "BufferVolumeData.h"
#include "BufferExtraData.h"

/**
 * 点データからVolumeDataへ変換するクラス
 */
class PointToVolume : public RefCount
{
private:
    RefPtr<BufferPointData>  m_point;
    RefPtr<BufferVolumeData> m_volume;

public:
    PointToVolume();
    ~PointToVolume();
    int Create(BufferPointData *point);

    ///
    /// Convert particles to volume. Resoltion of volume is set by resX, resY and resZ.
    /// @todo { ROI }
    ///
    bool ToVolume(int resX, int resY, int resZ);

    ///
    /// Convert particle attributes to volume.
    /// `attrib` must have same element size with position attribute `m_point`.
    /// Currently scalar or Vec3 type for `attrib` are supported.
    /// Crete scalar valued voxel volume for vec3 attrib data when `vector_length` is true.
    /// Resoltion of volume is set by resX, resY and resZ.
    /// @todo { ROI }
    ///
    bool ToVolumeWithAttrib(int resX, int resY, int resZ, BufferExtraData *attrib, bool vector_length);

#if 0
    ///
    /// Convert particles to volume. Resultion of volume is set by resX, resY and resZ.
    /// Can specify the ROI(region of interest) for particles to be voxelized..
    ///
    bool ToVolumeWithROI(int resX, int resY, int resZ);
#endif

    /// Valid after ToVolume()
    BufferVolumeData*  VolumeData();

};

#endif //_POINTTOVOLUME_H_

