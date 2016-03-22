/**
 * @file SolidAttribToVolume.h
 * Solid attrib データからVolumeDataへ変換するモジュール
 */

#ifndef _SOLIDATTRIBTOVOLUME_H_
#define _SOLIDATTRIBTOVOLUME_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferExtraData.h"
#include "BufferVolumeData.h"
#include "BufferSolidData.h"

/**
 * 多面体 attrib データからVolumeDataへ変換するクラス
 */
class SolidAttribToVolume : public RefCount
{
private:
    RefPtr<BufferSolidData>   m_solid;
    RefPtr<BufferExtraData>  m_attrib;
    RefPtr<BufferVolumeData> m_volume;
    
    float             m_bmin[3];
    float             m_bmax[3];
    
public:
    SolidAttribToVolume();
    ~SolidAttribToVolume();
    
    /// Assume bounding box computed a priori by the user(e.g. PolygonAnalyzer).
    int Create(BufferSolidData* solid, BufferExtraData* attrib, float bmin[3], float bmax[3]);
    
    /// Convert particles to volume. Resultion of volume is set by resX, resY and resZ.
    /// mode ... 0 : 0 order continuous , 1 : Primary succession , 2 n order continuous
    /// @todo { ROI }
    bool ToVolume(int resX, int resY, int resZ, int mode);
    
    /// Valid after ToVolume()
    BufferVolumeData*  VolumeData();
    
};

#endif //_SOLIDATTRIBTOVOLUME_H_

