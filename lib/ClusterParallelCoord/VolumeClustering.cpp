/**
 * @file VolumeClustering.cpp
 * ボリュームデータのクラスタリングモジュール
 */

#include <string>
#include "VolumeClustering.h"
#include "BufferVolumeData.h"
#include <math.h>
#include <float.h>


namespace {
    const int fNum = 1024;

    void getMinMax(BufferVolumeData* volume, std::vector<float>& minVal, std::vector<float>& maxVal) {
        const int w = volume->Width();
        const int h = volume->Height();
        const int d = volume->Depth();
        const int c = volume->Component();
        const float* buffer = static_cast<const float*>(volume->Buffer()->GetBuffer());

        for (int p = 0; p < c; ++p) {
            minVal[p] =  FLT_MAX;
            maxVal[p] = -FLT_MAX;        
            for (int i = 0; i < w * h * d; ++i) {
                const float v = buffer[c * i + p];
                minVal[p] = fmin(v, minVal[p]);
                maxVal[p] = fmax(v, maxVal[p]);                
            }
        }
    }

   
    void histgram(BufferVolumeData* volume, const std::vector<float>& minVal, const std::vector<float>& maxVal, const std::vector<float>& sigmaVal, std::vector< std::vector<float> >& f) {
        const int w = volume->Width();
        const int h = volume->Height();
        const int d = volume->Depth();
        const int c = volume->Component();
        const float* buffer = static_cast<const float*>(volume->Buffer()->GetBuffer());        
        for (int p = 0; p < c; ++p) {
            const float minMaxDiffP = maxVal[p] - minVal[p];
            for (int fn = 0; fn < fNum; ++fn) {
                for (int i = 0; i < w * h * d; ++i) {
                    const float v = buffer[c * i + p];
                    const float fidx = static_cast<float>(fn)+0.5;
                    const float vi = (fidx / fNum) * minMaxDiffP + minVal[p];
                    f[p][fn] += exp( - 0.5 * ( ( (v - vi)/sigmaVal[p] )*( (v - vi)/sigmaVal[p] ) )  );
                }
            }
        }
    }

    void clustering(const std::vector< std::vector<float> >& f, const std::vector<float>& minVal, const std::vector<float>& maxVal, std::vector< std::vector<VolumeClustering::Cluster> >& axisClusters) {
        const int compNum = f.size();        
        for (int p = 0; p < compNum; ++p) {
            VolumeClustering::Cluster cls;
            float v = f[p][0];
            cls.minIndex = 0;
            cls.maxIndex = 0;
            cls.topIndex = 0;
            
            enum GradMode {
                UP = 1,
                DOWN = -1
            };
            GradMode mode = UP;
            int i = 1;

            //printf("UP\n");
            while (i < fNum) {
                //printf("v=%f, f[%d][%d] = %f\n", v, p, i, f[p][i]);
                if (mode == UP && f[p][i] < v) {
                    cls.topIndex = i;
                    mode = DOWN;
                    //printf("DOWN\n");
                            
                }
                else if (mode == DOWN && f[p][i] > v) {
                    cls.maxIndex = i;                
                    mode = UP;
                    //printf("UP[%d] %d %d %d\n", p, cls.minIndex, cls.topIndex, cls.maxIndex);
                    axisClusters[p].push_back(cls);
                    cls.minIndex = i;
                    cls.maxIndex = i;
                    cls.topIndex = i;
                }
                v = f[p][i];
                ++i;                
            }
            //cls.topIndex = [TODO]; // TODO!!
            cls.maxIndex = fNum; // end
            axisClusters[p].push_back(cls);
        }

        // from index to value
        for (int p = 0; p < compNum; ++p) {
            float minMaxDiff = maxVal[p] - minVal[p];
            int cn = axisClusters[p].size();
            for (int c = 0; c < cn; ++c) {
                
                axisClusters[p][c].maxValue = (axisClusters[p][c].maxIndex / static_cast<float>(fNum)) * minMaxDiff + minVal[p];
                axisClusters[p][c].topValue = (axisClusters[p][c].topIndex / static_cast<float>(fNum)) * minMaxDiff + minVal[p];
                axisClusters[p][c].minValue = (axisClusters[p][c].minIndex / static_cast<float>(fNum)) * minMaxDiff + minVal[p];
                /*printf("axisCluster %d, %d, %d / %f, %f, %f\n",
                    axisClusters[p][c].minIndex, axisClusters[p][c].topIndex, axisClusters[p][c].maxIndex,
                    axisClusters[p][c].minValue, axisClusters[p][c].topValue, axisClusters[p][c].maxValue
                );*/                
            }
        }
    }

    void edgePower(BufferVolumeData* volume, const std::vector< std::vector<VolumeClustering::Cluster> >& axisClusters, std::vector< std::vector< std::vector<int> > >& edgeCount) {
        const int w = volume->Width();
        const int h = volume->Height();
        const int d = volume->Depth();
        const int comp = volume->Component();
        const float* buffer = static_cast<const float*>(volume->Buffer()->GetBuffer());
        
        for (int i = 0; i < w * h * d; ++i) {
            int cls[comp];
            for (int p = 0; p < comp; ++p) {                
                const float v = buffer[comp * i + p];
                int cl = 0;
                while(cl < axisClusters[p].size() && v > axisClusters[p][cl].maxValue) {
                    ++cl;
                }
                if (cl >= axisClusters[p].size()) {
                    //printf("Over max value:%f max=%f\n", v, axisClusters[p][cl-1].maxValue);
                    cl--;
                }
                cls[p] = cl;
            }
            for (int p = 0; p < comp - 1; ++p) {
                ++edgeCount[p][cls[p]][cls[p+1]];
            }
        }
    }

}


/// コンストラクタ
VolumeClustering::VolumeClustering()
{
    Clear();
    const float defaultSigma = 50.0 / fNum;
    const int maxAxis = 20;
    m_sigmaVal.resize(maxAxis);
    for (int i = 0; i < maxAxis; ++i) {
        m_sigmaVal[i] = defaultSigma;
    }
}

/// デストラクタ
VolumeClustering::~VolumeClustering()
{
}

/// メンバクリア
void VolumeClustering::Clear() {

}


int VolumeClustering::SetSigma(int axis, float sigma) {
    if (axis >= m_sigmaVal.size()) {
        return 0;
    }
    m_sigmaVal[axis] = sigma;
    return 1;
}


/**
 * ボリュームモデル解析
 * @param model 解析対象BufferVolumeData
 * @retval true 成功
 * @retval false 失敗
 */
bool VolumeClustering::Execute(BufferVolumeData* volume)
{
    if(!volume || volume->GetType() != BufferData::TYPE_VOLUME) {
        fprintf(stderr,"Volume data not found.");
        return false;
    }

    // init alloc
    m_hist.clear();
    m_minVal.clear();
    m_maxVal.clear();
    m_axisClusters.clear();
    m_edgeCounts.clear();
    
    const int compNum = volume->Component();    
    m_hist.resize(compNum);
    for (int i = 0; i < compNum; ++i) {
        m_hist[i].resize(fNum);
    }
    m_minVal.resize(compNum);
    m_maxVal.resize(compNum);
    m_axisClusters.resize(compNum);

#define DUMPVALUE 0

#if DUMPVALUE
    // dump volume info
    const int w = volume->Width();
    const int h = volume->Height();
    const int d = volume->Depth();
    const int c = volume->Component();
    printf("Volume Size = (%dx%dx%dx%d)\n", w, h, d, c); 
#endif

    // get min/max
    getMinMax(volume, m_minVal, m_maxVal);

#if DUMPVALUE    
    // dump min/max
    for (int p = 0; p < compNum; ++p) {
        printf("%d - Min = %f / Max = %f\n", p, m_minVal[p], m_maxVal[p]);
    }
#endif

    // histgram
    histgram(volume, m_minVal, m_maxVal, m_sigmaVal, m_hist);

#if DUMPVALUE
    // dump histgram
    for (int i = 0; i < fNum; ++i) {
        printf("%4d = %6.3f, %6.3f, %6.3f\n", i, m_hist[0][i], m_hist[1][i], m_hist[2][i]);
    }
#endif

    // clustering
    clustering(m_hist, m_minVal, m_maxVal, m_axisClusters);

#if DUMPVALUE
    // dump clustering
    for (int p = 0; p < 3; ++p) {
        int ax = m_axisClusters[p].size();
        for (int i = 0; i < ax; ++i) {
            printf("Cluster %3d-[%3d] = %6.3f, %6.3f, %6.3f\n", p, i, m_axisClusters[p][i].minValue, m_axisClusters[p][i].topValue, m_axisClusters[p][i].maxValue); 
        }
    }
#endif
    
    m_edgeCounts.resize(compNum - 1);
    for (int p = 0; p < compNum - 1; ++p) { // edge num = compNum - 1
        int pmax = m_axisClusters[p].size();
        m_edgeCounts[p].resize(pmax);
        for (int np = 0; np < pmax; ++np) {
            m_edgeCounts[p][np].resize(m_axisClusters[p+1].size());
        }
    
    }
    edgePower(volume, m_axisClusters, m_edgeCounts);

#if DUMPVALUE
    // dump edge value
    for (int p = 0; p < compNum - 1; ++p) {
        int en = m_edgeCounts[p].size();
        for (int i = 0; i < en; ++i) {
            int kn = m_edgeCounts[p][i].size();
            for (int k = 0; k < kn; ++k) {
                printf("Edge %3d[%3d]-%3d[%3d] = %d\n", p, i, p+1, k, m_edgeCounts[p][i][k]);
            }
        }
    }
#endif


    return true;
}

int VolumeClustering::GetAxisNum()
{
    return static_cast<int>(m_axisClusters.size());
}

int VolumeClustering::GetClusterNum(int axis)
{
    if (axis >= GetAxisNum()) {
        return 0;
    }
    return static_cast<int>(m_axisClusters[axis].size());
}

const VolumeClustering::Cluster& VolumeClustering::GetClusterValue(int axis, int cluster)
{
    static Cluster empty;
    if (axis >= GetAxisNum()) {
        printf("[Error] over axis num\n");
        return empty;
    }
    if (cluster >= GetClusterNum(axis)) {
        printf("[Error] over cluster num\n");
        return empty;
    }
    return m_axisClusters[axis][cluster];
}

int VolumeClustering::GetEdgePowers(int axis, int cluster, int nextCluster)
{    
     if (axis >= GetAxisNum() - 1) {
        printf("[Error] over axis num\n");
        return 0;
    }
    if (cluster >= GetClusterNum(axis)) {
        printf("[Error] over cluster num\n");
        return 0;
    }
    if (nextCluster >= GetClusterNum(axis + 1)) {
        printf("[Error] over cluster num\n");
        return 0;
    }

    const int cnt = m_edgeCounts[axis][cluster][nextCluster];
    return cnt;
}

float VolumeClustering::GetVolumeMin(int axis)
{
    if (axis >= GetAxisNum() - 1) {
        printf("[Error] over axis num\n");
        return 0.0f;
    }
    return m_minVal[axis];
}

float VolumeClustering::GetVolumeMax(int axis)
{
    if (axis >= GetAxisNum() - 1) {
        printf("[Error] over axis num\n");
        return 0.0f;
    }
    return m_maxVal[axis];
}

    