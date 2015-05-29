/**
 * @file VolumeAnalyzer.h
 * ボリュームデータ解析モジュール
 */
#ifndef _VOLUMEANALYZER_H_
#define _VOLUMEANALYZER_H_

#include <vector>
#include "Ref.h"
#include "VolumeModel.h"

/**
 * ボリュームデータ解析クラス
 */
class VolumeAnalyzer : public RefCount
{
private:
    double m_minVal[3];
    double m_maxVal[3];
    std::vector<float> m_volHist[3];
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
    const std::vector<float>& GetHistgram() const;
    bool Execute(VolumeModel*);
};


#endif //_VOLUMEANALYZER_LUA_H_

