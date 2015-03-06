#include <string>
#include "PolygonAnalyzer.h"
#include "Analyzer.h"
#include "../Buffer/BufferMeshData.h"
#include "Buffer.h"

PolygonAnalyzer::PolygonAnalyzer()
{
    Clear();
}

PolygonAnalyzer::~PolygonAnalyzer()
{
}

void PolygonAnalyzer::Clear() {
    m_minVal[0] = m_minVal[1] = m_minVal[2] = 0.0;
    m_maxVal[0] = m_maxVal[1] = m_maxVal[2] = 0.0;
}

double PolygonAnalyzer::MinX() {
    return m_minVal[0];
}

double PolygonAnalyzer::MinY() {
    return m_minVal[1];
}

double PolygonAnalyzer::MinZ() {
    return m_minVal[2];
}

double PolygonAnalyzer::MaxX() {
    return m_maxVal[0];
}

double PolygonAnalyzer::MaxY() {
    return m_maxVal[1];
}

double PolygonAnalyzer::MaxZ() {
    return m_maxVal[2];
}

bool PolygonAnalyzer::Execute(PolygonModel *model)
{
    PolygonAnalyzerProc proc;
    std::vector<float> m_volHist[3];
    if(model->GetMesh()) {
        int number = model->GetMesh()->Position()->GetNum();
        const float* buffer = static_cast<const float*>(model->GetMesh()->Position()->GetBuffer());
        if(number <= 0) {
            fprintf(stderr,"Mesh vertex number is ZERO.");
            return false;
        }
        proc.AnalyzeVector(m_volHist, m_minVal, m_maxVal, buffer, number);
    } else {
        fprintf(stderr,"Mesh data not found.");
    }
    return true;
}

