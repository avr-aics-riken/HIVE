#include "VolumeAnalyzer.h"
#include "Analyzer.h"
#include "../Buffer/BufferMeshData.h"

VolumeAnalyzer::VolumeAnalyzer()
{
    Clear();
}

VolumeAnalyzer::~VolumeAnalyzer()
{
}

void VolumeAnalyzer::Clear() {
    m_minVal[0] = m_minVal[1] = m_minVal[2] = 0.0;
    m_maxVal[0] = m_maxVal[1] = m_maxVal[2] = 0.0;
}

double VolumeAnalyzer::MinX() {
    return m_minVal[0];
}

double VolumeAnalyzer::MinY() {
    return m_minVal[1];
}

double VolumeAnalyzer::MinZ() {
    return m_minVal[2];
}

double VolumeAnalyzer::MaxX() {
    return m_maxVal[0];
}

double VolumeAnalyzer::MaxY() {
    return m_maxVal[1];
}

double VolumeAnalyzer::MaxZ() {
    return m_maxVal[2];
}

bool VolumeAnalyzer::Execute(VolumeModel *model)
{
    VolumeAnalyzerProc proc;
    std::vector<float> m_volHist[3];
    if(model->GetMesh()) {
        int number = model->GetVolume()->Position()->GetNum();
        const float* buffer = static_cast<const float*>(model->GetVolume()->Position()->GetBuffer());
        proc.AnalyzeVector(m_volHist, m_minVal, m_maxVal, buffer, number);
    } else {
        printf("%s : Mesh data not found.", __func__);
    }
    return true;
}

