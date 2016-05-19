/**
 * @file SolidToDFVolume.h
 * Solid データから SignedDistanceField へ変換するモジュール
 */

#ifndef _SOLIDTODFVOLUME_H_
#define _SOLIDTODFVOLUME_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"
#include "BufferSolidData.h"

/**
 * 多面体 データから SignedDistanceField へ変換するクラス
 */
class SolidToDFVolume : public RefCount
{
private:
    RefPtr<BufferSolidData>   m_solid;
    RefPtr<BufferVolumeData> m_volume;
    
    float             m_bmin[3];
    float             m_bmax[3];
    
public:
    SolidToDFVolume();
    ~SolidToDFVolume();
    
    /// Assume bounding box computed a priori by the user(e.g. PolygonAnalyzer).
    int Create(BufferSolidData* solid, float bmin[3], float bmax[3]);
    
    /// Convert particles to volume. Resultion of volume is set by resX, resY and resZ.
    /// @todo { ROI }
    bool ToVolume(int resX, int resY, int resZ);
    
    /// Valid after ToVolume()
    BufferVolumeData*  VolumeData();
    
};

#endif //_SOLIDTODFVOLUME_H_

