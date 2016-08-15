/**
 * @file VolumeClustering.h
 * ボリュームデータのクラスタリングモジュール
 */

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

#include <vector>

class VolumeClustering : public RefCount
{
public:
    class Cluster {
        public:
        Cluster() {
            maxIndex = 0;
            minIndex = 0;
            topIndex = 0;
            maxValue = 0.0;
            minValue = 0.0;
            topValue = 0.0;
        }
        int maxIndex;
        int minIndex;
        int topIndex;
        
        float maxValue;
        float minValue;
        float topValue;
    };

public:
    VolumeClustering();
    ~VolumeClustering();

    void Clear();

    bool Execute(BufferVolumeData* volume);

    int GetAxisNum();
    int GetClusterNum(int axis);
    const Cluster& GetClusterValue(int axis, int cluster);
    
private:
    std::vector< std::vector<float> > m_hist;
    std::vector<float> m_minVal;
    std::vector<float> m_maxVal;
    std::vector< std::vector<Cluster> > m_axisClusters;
};

