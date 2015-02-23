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
    BufferLineData*   m_line;
    double            m_pitchX;
    double            m_pitchY;
    double            m_pitchZ;
public:
    VolumeToVector();
    int Create(BufferVolumeData *volume, double scale, double radius);
    BufferTetraData* TetraData();
    BufferLineData*  LineData();

    void DividePitchX(double a) ;
    void DividePitchY(double a) ;
    void DividePitchZ(double a) ;
    void DivideNumberX(double a);
    void DivideNumberY(double a);
    void DivideNumberZ(double a);
};

#endif //_VOLUMETOVECTOR_H_

