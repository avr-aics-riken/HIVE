/**
 * @file VolumeAnalyzer.h
 * ボリュームデータ解析モジュール
 */
#ifndef _VOLUMEANALYZER_H_
#define _VOLUMEANALYZER_H_

#include <vector>
#include "Ref.h"
class BufferVolumeData;

/**
 * ボリュームデータ解析クラス
 */
class VolumeAnalyzer : public RefCount
{
private:
    double m_minVal[4];
    double m_maxVal[4];
    std::vector<float> m_volHist[4];
    void Clear();
public:
    VolumeAnalyzer();
    ~VolumeAnalyzer();
    double MinX();
    double MinY();
    double MinZ();
    double MinW();
    double MaxX();
    double MaxY();
    double MaxZ();
    double MaxW();
    const std::vector<float>& GetHistgram() const;
    const std::vector<float> GetHistgramInRange(BufferVolumeData*, double min, double max) const;
    bool Execute(BufferVolumeData*);
};


#endif //_VOLUMEANALYZER_LUA_H_

