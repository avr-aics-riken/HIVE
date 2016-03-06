/**
 * @file SparseVolumeAnalyzer.h
 * ボリュームデータ解析モジュール
 */
#ifndef _SPARSEVOLUMEANALYZER_H_
#define _SPARSEVOLUMEANALYZER_H_

#include <vector>
#include "Ref.h"
//#include "SparseVolumeModel.h"
class SparseVolumeModel;

/**
 * ボリュームデータ解析クラス
 */
class SparseVolumeAnalyzer : public RefCount
{
private:
    double m_minVal[3];
    double m_maxVal[3];
    std::vector<float> m_volHist[3];
    void Clear();
public:
    SparseVolumeAnalyzer();
    ~SparseVolumeAnalyzer();
    double MinX();
    double MinY();
    double MinZ();
    double MaxX();
    double MaxY();
    double MaxZ();
    const std::vector<float>& GetHistgram() const;
    const std::vector<float> GetHistgramInRange(SparseVolumeModel*, double min, double max) const;
    bool Execute(SparseVolumeModel*);
};


#endif //_SPARSEVOLUMEANALYZER_LUA_H_

