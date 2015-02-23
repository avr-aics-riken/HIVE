#ifndef _VOLUMETOVECTOR_H_
#define _VOLUMETOVECTOR_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>
#include "BufferVolumeData.h"
#include "BufferTetraData.h"
#include "BufferLineData.h"

class VolumeToVector : public RefCount
{
private:
    BufferTetraData*  m_tetra;
    BufferLineData*    m_line;
public:
    VolumeToVector();
    int Create(BufferVolumeData *volume, double scale, double radius);
    BufferTetraData* TetraData();
    BufferLineData*  LineData();
};

#endif //_VOLUMETOVECTOR_H_

