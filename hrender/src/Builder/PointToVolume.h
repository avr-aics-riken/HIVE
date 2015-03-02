#ifndef _POINTTOVOLUME_H_
#define _POINTTOVOLUME_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferPointData.h"
#include "BufferVolumeData.h"

class PointToVolume : public RefCount
{
private:
    BufferPointData*  m_point;
    BufferVolumeData* m_volume;

public:
    PointToVolume();
    ~PointToVolume();
    int Create(BufferPointData *point);

    /// Convert particles to volume. Resultion of volume is set by resX, resY and resZ.
    /// @todo { ROI }
    bool ToVolume(int resX, int resY, int resZ);

    /// Valid after ToVolume()
    BufferVolumeData*  VolumeData();

};

#endif //_POINTTOVOLUME_H_

