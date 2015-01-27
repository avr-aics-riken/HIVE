#include "PolygonAnalyzer.h"
#include "Analyzer.h"

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
    float m_volHist[3] = {0,0,0};
    const float *volumedata = NULL;
    int number  = 0;
    if(model->GetMesh()) {
        number     = model->GetMesh()->Position()->GetNum();
        volumedata = model->GetMesh()->Position()->GetBuffer();
        printf("%s : number = %08X\n", __func__, number);
        printf("%s : buffer = %08X\n", __func__, volumedata);
        //proc.AnalyzeVector(m_volHist, m_minVal, m_maxVal, volumedata, number);
    } else {
        printf("%s : Mesh data not found.", __func__);
    }
    return true;
}

