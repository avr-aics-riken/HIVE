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
    
    return m_volume->Width() * m_volume->Height() * m_volume->Depth();
}

BufferVolumeData* VolumeQuantizer::VolumeData()
{
    return m_volume;
}

