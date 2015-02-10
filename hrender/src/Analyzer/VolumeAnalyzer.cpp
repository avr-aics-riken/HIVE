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
            printf("Volume data empty\n");
            return false;
        }
        proc.AnalyzeVector(m_volHist, m_minVal, m_maxVal, buffer, temp_num);
    } else {
        printf("Volume data not found.");
    }
    return true;
}

