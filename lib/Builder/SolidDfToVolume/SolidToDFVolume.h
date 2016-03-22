/**
 * @file SolidAttribToVolume.h
 * Solid データから作った SignedDistanceField を VolumeDataへ変換するモジュール
 */

#ifndef SOLIDDFTOVOLUME_H_
#define SOLIDDFTOVOLUME_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"
#include "BufferSolidData.h"

/**
 * 多面体データから作った SDF を VolumeDataに変換するクラス
 */
class SolidDfToVolume : public RefCount
{
private:
    RefPtr<BufferSolidData>   m_solid;
    RefPtr<BufferVolumeData>  m_volume;
    
    float             m_bmin[3];
    float             m_bmax[3];
    
public:
    SolidDfToVolume();
    ~SolidDfToVolume();
    
    /// Assume bounding box computed a priori by the user(e.g. PolygonAnalyzer).
    int Create(BufferSolidData* solid, float bmin[3], float bmax[3]);
    
    /// Convert particles to volume. Resultion of volume is set by resX, resY and resZ.
    /// @todo { ROI }
    bool ToVolume(int resX, int resY, int resZ);
    
    /// Valid after ToVolume()
    BufferVolumeData*  VolumeData();
    
};