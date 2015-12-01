/**
 * @file VolumeToVector.h
 * VolumeDataからVectorメッシュデータを作成するモジュール
 */
#ifndef _VOLUMETOVECTOR_H_
#define _VOLUMETOVECTOR_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>
#include "BufferVolumeData.h"
#include "BufferTetraData.h"
#include "BufferLineData.h"
#include "BufferVectorData.h"

/**
 * VolumeDataからVectorメッシュデータを作成するモジュール
 */
class VolumeToVector : public RefCount
{
private:
    BufferVectorData* m_vector;
    double            m_pitchX;
    double            m_pitchY;
    double            m_pitchZ;
    int               m_pitchIntX;
    int               m_pitchIntY;
    int               m_pitchIntZ;
    bool              m_usePitchInt;

public:
    VolumeToVector();
    int Create(BufferVolumeData *volume);
    BufferVectorData*  VectorData();

    bool DividePitchX(double a) ;
    bool DividePitchY(double a) ;
    bool DividePitchZ(double a) ;
    bool DividePitch(double x, double y, double z);
    bool DivideNumberX(int a);
    bool DivideNumberY(int a);
    bool DivideNumberZ(int a);
    bool DivideNumber(int x, int y, int z);

};

#endif //_VOLUMETOVECTOR_H_

