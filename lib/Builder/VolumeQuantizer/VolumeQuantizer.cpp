/**
 * @file VolumeToVector.cpp  
 * VolumeDataからVectorメッシュデータを作成するモジュール
 */
#include "VolumeQuantizer.h"
#include "../Core/vxmath.h"
#include <string.h>




/// コンストラクタ
VolumeQuantizer::VolumeQuantizer(){
    m_volume      = BufferVolumeData::CreateInstance();
    m_quantizeSize = 1024;
}

bool VolumeQuantizer::QuantizeSize(int qsize) {
    if(qsize <= 1) {
        printf("ERROR : Invalid QuantizeSize\n");
        return false;
    }
    m_quantizeSize = qsize;
    return true;
}

int VolumeQuantizer::Create(BufferVolumeData *volume) {
    if (!volume) {
        return 0;
    }
    
    const int qsize = m_quantizeSize;
    const int c = volume->Component();
    const int w = volume->Width();
    const int h = volume->Height();
    const int d = volume->Depth();
    const FloatBuffer* vol = volume->Buffer();
    
    // calc min/max
    for (int e = 0; e < c; ++e) {
        double emin = 1e+39, emax = -1e+39;
        for (int z = 0; z < d; ++z) {
            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    const float v = vol->GetBuffer()[c * (x + y * w + z * w * h) + e];
                    emin = emin > v ? v : emin;
                    emax = emax < v ? v : emax;
                }
            }
        }
        m_minmax.push_back(std::make_pair(emin, emax));
        printf("[%d] min=%f, max=%f\n", e, emin, emax);
    }
    
    m_volume->Create(qsize, 1, 1, c);
    float* fb = m_volume->Buffer()->GetBuffer();
    memset(fb, 0, qsize * c * sizeof(float));
    
    for (int e = 0; e < c; ++e) {
        double emin = 1e+39, emax = -1e+39;
        for (int z = 0; z < d; ++z) {
            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    const float v = vol->GetBuffer()[c * (x + y * w + z * w * h) + e];
                    const int q = static_cast<int>((v - emin) / (emax - emin) * qsize);
                    fb[c * q + e] += 1.0;
                }
            }
        }
    }
    
    return m_volume->Width() * m_volume->Height() * m_volume->Depth();
}

BufferVolumeData* VolumeQuantizer::VolumeData()
{
    return m_volume;
}

const std::vector<std::pair<float, float> >& VolumeQuantizer::GetMinMax()
{
    return m_minmax;
}

