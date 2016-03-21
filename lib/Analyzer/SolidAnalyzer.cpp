/**
 * @file SolidAnalyzer.cpp
 * ソリッドデータ解析モジュール
 */

#include <string>
#include "SolidAnalyzer.h"
#include "Analyzer.h"
#include "../Buffer/BufferSolidData.h"
#include "Buffer.h"

/// コンストラクタ
SolidAnalyzer::SolidAnalyzer()
{
    Clear();
}

/// デストラクタ
SolidAnalyzer::~SolidAnalyzer()
{
}

/// メンバクリア
void SolidAnalyzer::Clear() {
    m_minVal[0] = m_minVal[1] = m_minVal[2] = 0.0;
    m_maxVal[0] = m_maxVal[1] = m_maxVal[2] = 0.0;
}

/**
 * 解析したソリッドデータX軸最小値
 * @return X軸最小値
 */
double SolidAnalyzer::MinX() {
    return m_minVal[0];
}

/**
 * 解析したソリッドデータY軸最小値
 * @return Y軸最小値
 */
double SolidAnalyzer::MinY() {
    return m_minVal[1];
}

/**
 * 解析したソリッドデータZ軸最小値
 * @return Z軸最小値
 */
double SolidAnalyzer::MinZ() {
    return m_minVal[2];
}

/**
 * 解析したソリッドデータZ軸最大値
 * @return X軸最大値
 */
double SolidAnalyzer::MaxX() {
    return m_maxVal[0];
}

/**
 * 解析したソリッドデータZ軸最大値
 * @return Y軸最大値
 */
double SolidAnalyzer::MaxY() {
    return m_maxVal[1];
}

/**
 * 解析したソリッドデータZ軸最大値
 * @return Z軸最大値
 */
double SolidAnalyzer::MaxZ() {
    return m_maxVal[2];
}


/**
 * ソリッドモデル解析
 * @param model 解析対象BufferSolidData
 * @retval true 成功
 * @retval false 失敗
 */
bool SolidAnalyzer::Execute(BufferSolidData* solid)
{
    std::vector<float> m_volHist[3];
    
    if(!solid || solid->GetType() != BufferData::TYPE_SOLID) {
        fprintf(stderr,"Solid data not found.");
        return false;
    }
    
    int number = solid->Position()->GetNum();
    const float* buffer = static_cast<const float*>(solid->Position()->GetBuffer());
    if (number <= 0) {
        fprintf(stderr,"The number of vertices is ZERO.");
        return false;
    }
    
		// Reuse Polygon analyzer.
    PolygonAnalyzerProc proc;
    proc.AnalyzeVector(m_volHist, m_minVal, m_maxVal, buffer, number);
    
    return true;
}

