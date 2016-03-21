/**
 * @file SolidToVolume.h
 * SolidデータからVolumeDataへ変換するモジュール
 */

#ifndef SOLIDTOVOLUME_H
#define SOLIDTOVOLUME_H

#include "Ref.h"
#include "Buffer.h"
#include "BufferSolidData.h"
#include "BufferVolumeData.h"

/**
 * SolidデータからVolumeDataへ変換するクラス (in_out)
 */
class SolidToVolume : public RefCount
{
private:
    BufferSolidData*  m_solid;
    BufferVolumeData* m_volume;
    float             m_bmin[3];
    float             m_bmax[3];
    
public:
    SolidToVolume();
    ~SolidToVolume();
    int Create(BufferSolidData *point, float bmin[3], float bmax[3]);
    
    /// Convert particles to volume. Resultion of volume is set by resX, resY and resZ.
    /// @todo { ROI }
    bool ToVolume(int resX, int resY, int resZ);
    
    /// Valid after ToVolume()
    BufferVolumeData*  VolumeData();
    
};

#endif //_SOLIDTOVOLUME_H_

