/**
 * @file MandelbulbVolGen.cpp
 * Mandelbulb Volume Generator
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "MandelbulbVolGen.h"

/// Constructor
MandelbulbVolGen::MandelbulbVolGen()
{
    m_volume = BufferVolumeData::CreateInstance();
    Clear();
}
/// Destructor
MandelbulbVolGen::~MandelbulbVolGen()
{
    Clear();
}

/// Clear members
void MandelbulbVolGen::Clear()
{
    m_volume->Clear();
}

void mandelbulb(const float px, const float py, const float pz,
                float &iterationCount, float &minDist, float &maxDist)
{
    const int maxIteration = 4;
    const float power = 8.;
    iterationCount = 0.;
    float r = 0;
    float z[3] = {px, py, pz};
    minDist = 99999;
    maxDist = -1;
    for (int i = 0; i < maxIteration; i++) {
		iterationCount = i;
        r = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
        minDist = std::min(minDist, r);
        maxDist = std::max(maxDist, r);
        if( r > 2. ) {
            minDist = 0;
            maxDist = 0;
            iterationCount = 0;
            return;
        }
        
        float zr = pow( r, power);
        float theta = acos(z[1] / r);
		float phi = atan2(z[2], z[0]);
        theta *= power;
        phi *= power;
        z[0] = zr * sin(theta)*cos(phi) + px;
        z[1] = zr * cos(theta) + py;
        z[2] = zr * sin(phi) * sin(theta) + pz;
    }
}

float* calcMandelbulb(const int w, const int h, const int d)
{
    float* voldata = new float [w * h * d * 1];
    float* p = voldata;
    
    float bmin[3] = {-1.5, -1.5, -1.5};
    float bmax[3] = {1.5, 1.5, 1.5};
    float step[3] = {(bmax[0] - bmin[0]) / w,
                     (bmax[1] - bmin[1]) / h,
                     (bmax[2] - bmin[2]) / d};
    float offset[3] = {step[0] / 2., step[1] / 2., step[2] / 2.};

    for (int vz = 0 ; vz < d ; vz++) {
        float z = step[2] * vz + bmin[2] + offset[2];
        for (int vy = 0 ; vy < h ; vy++) {
            float y = step[1] * vy + bmin[1] + offset[1];
            for (int vx = 0 ; vx < w ; vx++) {
                float x = step[0] * vx + bmin[0] + offset[0];
                float iterationCount, minDist, maxDist;
                mandelbulb(x, y, z,
                           iterationCount, minDist, maxDist);
                *p = minDist;
                p++;
                // *p = maxDist;
                // p++;
                // *p = maxDist;
                // p++;
            }
        }
    }
    return voldata;
}

/**
 * Generate Mandelbulb Volume
 * @param size size
 * @retval true 成功
 * @retval false 失敗
 */
bool MandelbulbVolGen::Generate(const int size)
{
    Clear();
    fprintf(stderr,"size: %d\n", size);

    // resolution ... size * size * size
    const int w = size;
    const int h = size;
    const int d = size;
    // Mandelbulb Volume has 1 components
    // Use minDist as density
    const int c = 1;
    const float* buf = calcMandelbulb(w, h, d);
    m_volume->Create(w, h, d, c);
    const int fnum = w * h * d * c;
    memcpy(m_volume->Buffer()->GetBuffer(), buf, fnum * sizeof(float));
    delete [] buf;
    
    return true;
}

/**
 * VOLWidth取得
 * @retval int Width
 */
int MandelbulbVolGen::Width()
{
    return m_volume->Width();
}

/**
 * VOLHeight取得
 * @retval int Height
 */
int MandelbulbVolGen::Height()
{
    return m_volume->Height();
}

/**
 * VOLDepth取得
 * @retval int Depth
 */
int MandelbulbVolGen::Depth()
{
    return m_volume->Depth();
}

/**
 * VOLComponent取得
 * @retval int Component数
 */
int MandelbulbVolGen::Component()
{
    return m_volume->Component();
}

/**
 * VOLデータバッファ参照取得
 * @retval FloatBuffer* FloatBufferアドレス
 */
FloatBuffer* MandelbulbVolGen::Buffer()
{
    return m_volume->Buffer();
}

/**
 * VolumeData参照取得
 * @retval BufferVolumeData* VolumeData参照
 */
BufferVolumeData *MandelbulbVolGen::VolumeData()
{
    return m_volume;
}

