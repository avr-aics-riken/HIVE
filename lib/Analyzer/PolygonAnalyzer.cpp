/**
 * @file PolygonAnalyzer.cpp
 * ポリゴンデータ解析モジュール
 */

#include <string>
#include "PolygonAnalyzer.h"
#include "Analyzer.h"
#include "../Buffer/BufferMeshData.h"
#include "Buffer.h"

/// コンストラクタ
PolygonAnalyzer::PolygonAnalyzer()
{
    Clear();
}

/// デストラクタ
PolygonAnalyzer::~PolygonAnalyzer()
{
}

/// メンバクリア
void PolygonAnalyzer::Clear() {
    m_minVal[0] = m_minVal[1] = m_minVal[2] = 0.0;
    m_maxVal[0] = m_maxVal[1] = m_maxVal[2] = 0.0;
}

/**
 * 解析したポリゴンデータX軸最小値
 * @return X軸最小値
 */
double PolygonAnalyzer::MinX() {
    return m_minVal[0];
}

/**
 * 解析したポリゴンデータY軸最小値
 * @return Y軸最小値
 */
double PolygonAnalyzer::MinY() {
    return m_minVal[1];
}

/**
 * 解析したポリゴンデータZ軸最小値
 * @return Z軸最小値
 */
double PolygonAnalyzer::MinZ() {
    return m_minVal[2];
}

/**
 * 解析したポリゴンデータZ軸最大値
 * @return X軸最大値
 */
double PolygonAnalyzer::MaxX() {
    return m_maxVal[0];
}

/**
 * 解析したポリゴンデータZ軸最大値
 * @return Y軸最大値
 */
double PolygonAnalyzer::MaxY() {
    return m_maxVal[1];
}

/**
 * 解析したポリゴンデータZ軸最大値
 * @return Z軸最大値
 */
double PolygonAnalyzer::MaxZ() {
    return m_maxVal[2];
}


/**
 * ポリゴンモデル解析
 * @param model 解析対象BufferMeshData
 * @retval true 成功
 * @retval false 失敗
 */
bool PolygonAnalyzer::Execute(BufferMeshData* mesh)
{
    std::vector<float> m_volHist[3];
    
    if(!mesh || mesh->GetType() != BufferData::TYPE_MESH) {
        fprintf(stderr,"Mesh data not found.");
        return false;
    }
    
    int number = mesh->Position()->GetNum();
    const float* buffer = static_cast<const float*>(mesh->Position()->GetBuffer());
    if (number <= 0) {
        fprintf(stderr,"Mesh vertex number is ZERO.");
        return false;
    }
    
    PolygonAnalyzerProc proc;
    proc.AnalyzeVector(m_volHist, m_minVal, m_maxVal, buffer, number);
    
    return true;
}

