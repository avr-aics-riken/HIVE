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
    m_quantizeSize = 8;
    m_sampling[0] = 10;
    m_sampling[1] = 10;
    m_sampling[2] = 10;
    m_elementSize = 0;
}

bool VolumeQuantizer::QuantizeSize(int qsize) {
    if(qsize <= 1) {
        printf("ERROR : Invalid QuantizeSize\n");
        return false;
    }
    m_quantizeSize = qsize;
    return true;
}

bool VolumeQuantizer::SamplingNum(int widthDiv, int heightDiv, int depthDiv) {
    if(widthDiv <= 0 || heightDiv <= 0 || depthDiv <= 0) {
        printf("ERROR : Invalid Sampling Size\n");
        return false;
    }
    m_sampling[0] = widthDiv;
    m_sampling[1] = heightDiv;
    m_sampling[2] = depthDiv;
    return true;
}

bool VolumeQuantizer::Clear()
{
    m_custom_minmax.clear();
    m_inputs.clear();
    return true;
}

bool VolumeQuantizer::Add(BufferVolumeData *volume)
{
    if (!volume) {
        return false;
    }
    if (m_inputs.size() > 0) {
        if (m_inputs[0]->Width()  != volume->Width()
        ||  m_inputs[0]->Height() != volume->Height()
        ||  m_inputs[0]->Depth()  != volume->Depth()) {
            return false;
        }
    }
    m_inputs.push_back(volume);
    return true;
}

bool VolumeQuantizer::AddCustomMinMax(BufferVolumeData *volume, float minval, float maxval)
{
    bool r = Add(volume);
    if (!r) {
        return false;
    }
    
    m_custom_minmax[volume] = std::make_pair(minval, maxval);
    return true;
}


namespace {
    int getComponentSize(int bits, int comp) {
        int outbit = 0;
        const int alignBit = 32;
        for (int c = 0; c < comp; ++c) {
            if (outbit / alignBit < (outbit + bits) / alignBit ) {
                outbit = (outbit / alignBit + 1) * alignBit;
            }
            outbit += bits;
        }
        if (outbit % alignBit != 0) {
            outbit = (outbit / alignBit + 1) * alignBit;
        }
        return outbit / 8;
    }
}

int VolumeQuantizer::Create()
{
    if (m_inputs.size() <= 0) {
        return 0;
    }
    const int depth   = m_inputs[0]->Depth();
    const int height  = m_inputs[0]->Height();
    const int width   = m_inputs[0]->Width();
    const int bits    = m_quantizeSize;
    const int datanum = static_cast<int>(m_inputs.size());
    
    // calc min/max
    m_minmax.clear();
    for (int i = 0; i < datanum; ++i) {
        BufferVolumeData* v = m_inputs[i];
        std::map< BufferVolumeData*,std::pair<float,float> >::iterator it = m_custom_minmax.find(v);
        double emin = 1e+39, emax = -1e+39;
        if (it != m_custom_minmax.end()) {
            emin = it->second.first;
            emax = it->second.second;
        } else {
            int c = m_inputs[i]->Component();
            FloatBuffer* vol = m_inputs[i]->Buffer();
            for (int z = 0; z < depth; ++z) {
                for (int y = 0; y < height; ++y) {
                    for (int x = 0; x < width; ++x) {
                        const float v = vol->GetBuffer()[c * (x + y * width + z * width * height)];
                        emin = emin > v ? v : emin;
                        emax = emax < v ? v : emax;
                    }
                }
            }
        }
        m_minmax.push_back(std::make_pair(emin, emax));
        printf("[%d] min=%f, max=%f\n", i, emin, emax);
    }
    
    
    // calc size
    int step_w = width  / m_sampling[0]; step_w = step_w <= 0 ? 1 : step_w;
    int step_h = height / m_sampling[1]; step_h = step_h <= 0 ? 1 : step_h;
    int step_d = depth  / m_sampling[2]; step_d = step_d <= 0 ? 1 : step_d;
    const int comp   = getComponentSize(bits, m_inputs.size());
    m_elementSize = comp;

    // alloc new buffer
    m_volume->Create(m_sampling[0], m_sampling[1], m_sampling[2], comp);
    const int tarW = m_volume->Width();
    const int tarH = m_volume->Height();
    const int tarD = m_volume->Depth();
    printf("targetSize = (%d,%d,%d)\n", tarW, tarH, tarD);
    float* tarBuf = m_volume->Buffer()->GetBuffer();
    
    // packing
    const float bitVal = pow(2.0, bits);
    int p = 0;
    for (int z = 0; z < depth; z += step_d) {
        for (int y = 0; y < height; y += step_h) {
            for (int x = 0; x < width; x += step_w) {
                unsigned int pack = 0;
                for (int i = 0; i < datanum; ++i) {
                    FloatBuffer* fb = m_inputs[i]->Buffer();
                    const int c = m_inputs[i]->Component();
                    const float v = fb->GetBuffer()[(x + y * width + z * width * height) * c];
                    const float emin = m_minmax[i].first;
                    const float emax = m_minmax[i].second;
                    const int q = static_cast<int>((v - emin) / (emax - emin) * bitVal);
                    //printf("Q=%d\n",q);
                    pack |= q << (i * bits); // 32bit x4
                    tarBuf[p] = *reinterpret_cast<float*>(&pack);
                    ++p;
                }
            }
        }
    }
    return tarW * tarH * tarD * comp * sizeof(unsigned int);
}
/*

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
    
    m_volume->Create(w/2, h/2, d/2, 1);// 32bit
    float* fb = m_volume->Buffer()->GetBuffer();
    memset(fb, 0, qsize * c * sizeof(float));
    
    for (int z = 0; z < d/2; ++z) {
        for (int y = 0; y < h/2; ++y) {
            for (int x = 0; x < w/2; ++x) {
                unsigned int eq = 0;
                for (int e = 0; e < c; ++e) {
                    float emin = m_minmax[e].first, emax = m_minmax[e].second;
                    const float v = vol->GetBuffer()[c * (x + y * w + z * w * h) + e];
                    const int q = static_cast<int>((v - emin) / (emax - emin) * qsize);
                    eq |= q << (e * 8); // 32bit x4
                    fb[x + y * w/2 + z * w/2 * h/2] = *reinterpret_cast<float*>(&eq);
                }
            }
        }
    }
    
    return m_volume->Width() * m_volume->Height() * m_volume->Depth();
}
*/

BufferVolumeData* VolumeQuantizer::VolumeData()
{
    return m_volume;
}

const std::vector<std::pair<float, float> >& VolumeQuantizer::GetMinMax()
{
    return m_minmax;
}

int VolumeQuantizer::DataElementSize()
{
    return m_elementSize;
}
