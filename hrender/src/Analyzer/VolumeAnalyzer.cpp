/**
 * @file VolumeAnalyzer.cpp
 * ボリュームデータ解析モジュール
 */

#include "VolumeAnalyzer.h"
#include "Analyzer.h"
#include "../Buffer/BufferMeshData.h"
#include "Buffer.h"

/// コンストラクタ
VolumeAnalyzer::VolumeAnalyzer()
{
    Clear();
}

/// デストラクタ
VolumeAnalyzer::~VolumeAnalyzer()
{
}

/// メンバクリア
void VolumeAnalyzer::Clear() {
    m_minVal[0] = m_minVal[1] = m_minVal[2] = 0.0;
    m_maxVal[0] = m_maxVal[1] = m_maxVal[2] = 0.0;
}

/**
 * 解析したボリュームデータX軸最小値
 * @return X軸最小値
 */
double VolumeAnalyzer::MinX() {
    return m_minVal[0];
}

/**
 * 解析したボリュームデータX軸最小値
 * @return X軸最小値
 */
double VolumeAnalyzer::MinY() {
    return m_minVal[1];
}

/**
 * 解析したボリュームデータX軸最小値
 * @return X軸最小値
 */
double VolumeAnalyzer::MinZ() {
    return m_minVal[2];
}

/**
 * 解析したボリュームデータX軸最大値
 * @return X軸最大値
 */
double VolumeAnalyzer::MaxX() {
    return m_maxVal[0];
}

/**
 * 解析したボリュームデータY軸最大値
 * @return Y軸最大値
 */
double VolumeAnalyzer::MaxY() {
    return m_maxVal[1];
}

/**
 * 解析したボリュームデータZ軸最大値
 * @return Z軸最大値
 */
double VolumeAnalyzer::MaxZ() {
    return m_maxVal[2];
}

/**
 * ボリュームモデル解析
 * @param model 解析対象PolygonModel
 * @retval true 成功
 * @retval false 失敗
 */
bool VolumeAnalyzer::Execute(VolumeModel *model)
{
    VolumeAnalyzerProc proc;
    std::vector<float> m_volHist[3];
    if(model->GetVolume()) {
        BufferVolumeData *volume = model->GetVolume();
        const float* buffer = static_cast<const float*>(volume->Buffer()->GetBuffer());

        int temp_num[3] = {
            volume->Width(),
            volume->Height(),
            volume->Depth()
        };

        const int fnum = temp_num[0] + temp_num[1] + temp_num[2];
        if(fnum <= 0)
        {
            fprintf(stderr,"Volume data empty\n");
            return false;
        }
        
        if (volume->Component() == 1) {
            proc.AnalyzeScalar(m_volHist[0], m_minVal[0], m_maxVal[0], buffer, temp_num);
        } else if (volume->Component() == 3) {
            proc.AnalyzeVector(m_volHist, m_minVal, m_maxVal, buffer, temp_num);
        } else {
            fprintf(stderr,"# of components in the volume cell must be 1 or 3.\n");
        }
    } else {
        fprintf(stderr,"Volume data not found.");
    }
    return true;
}

