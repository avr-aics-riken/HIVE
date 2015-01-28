#ifndef _VOLUMEANALYZER_H_
#define _VOLUMEANALYZER_H_

#include "Ref.h"
#include "VolumeModel.h"

class VolumeAnalyzer : public RefCount
{
private:
    double m_minVal[3];
    double m_maxVal[3];
    void Clear();
public:
    VolumeAnalyzer();
    ~VolumeAnalyzer();
    double MinX();
    double MinY();
    double MinZ();
    double MaxX();
    double MaxY();
    double MaxZ();
    bool Execute(VolumeModel*);
};


#endif //_VOLUMEANALYZER_LUA_H_

