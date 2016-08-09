/**
 * @file VolumeClustering.cpp
 * ボリュームデータのクラスタリングモジュール
 */

#include <string>
#include "VolumeClustering.h"
#include "BufferVolumeData.h"
#include <math.h>

/// コンストラクタ
VolumeClustering::VolumeClustering()
{
    Clear();
}

/// デストラクタ
VolumeClustering::~VolumeClustering()
{
}

/// メンバクリア
void VolumeClustering::Clear() {

}

/**
 * ボリュームモデル解析
 * @param model 解析対象BufferVolumeData
 * @retval true 成功
 * @retval false 失敗
 */
bool VolumeClustering::Execute(BufferVolumeData* volume)
{
    if(!volume || volume->GetType() != BufferData::TYPE_VOLUME) {
        fprintf(stderr,"Volume data not found.");
        return false;
    }

    const int pNum = 3;    
    const int fNum = 1024;
    float f[pNum][fNum];
    for (int p = 0; p < pNum; ++p) {
        for (int i = 0; i < fNum; ++i) {
            f[p][i] = 0.0f;
        }
    }

    const int w = volume->Width();
    const int h = volume->Height();
    const int d = volume->Depth();
    const int c = volume->Component();
    const float* buffer = static_cast<const float*>(volume->Buffer()->GetBuffer());
    
    printf("Volume Size = (%dx%dx%dx%d)\n", w, h, d, c); 

    // get min/max
    float minVal[pNum];
    float maxVal[pNum];
    for (int p = 0; p < pNum; ++p) {
        minVal[p] =  MAXFLOAT;
        maxVal[p] = -MAXFLOAT;        
        for (int i = 0; i < w * h * d; ++i) {
            const float v = buffer[c * i + p];
            minVal[p] = fmin(v, minVal[p]);
            maxVal[p] = fmax(v, maxVal[p]);                
        }
    }

    for (int p = 0; p < pNum; ++p) {
        printf("%d - Min = %f / Max = %f\n", p, minVal[p], maxVal[p]);
    }

    const float sigma = 10.0 / fNum;
    for (int p = 0; p < pNum; ++p) {
        const float minMaxDiffP = maxVal[p] - minVal[p];
        for (int fn = 0; fn < fNum; ++fn) {
            for (int i = 0; i < w * h * d; ++i) {
                const float v = buffer[c * i + p];
                const float fidx = static_cast<float>(fn)+0.5;
                const float vi = (fidx / fNum) * minMaxDiffP + minVal[p];
                f[p][fn] += exp( - 0.5 * ( ( (v - vi)/sigma )*( (v - vi)/sigma ) )  );
            }
        }
    }
    for (int i = 0; i < fNum; ++i) {
        printf("%4d = %6.3f, %6.3f, %6.3f\n", i, f[0][i], f[1][i], f[2][i]);
    }
    return true;
}

