/**
 * @file SparseVolumeAnalyzer.cpp
 * ボリュームデータ解析モジュール
 */

#include "SparseVolumeAnalyzer.h"
#include "Analyzer.h"
#include "../Buffer/BufferSparseVolumeData.h"
#include "../Buffer/BufferVolumeData.h"
#include "Buffer.h"

namespace {

void FindMinMax(double minVal[3], double maxVal[3], BufferSparseVolumeData* volume)
{
    // First find min/max
    minVal[0] = minVal[1] = minVal[2] = std::numeric_limits<double>::max();
    maxVal[0] = maxVal[1] = maxVal[2] = -std::numeric_limits<double>::max();

    for (size_t i = 0; i < volume->VolumeBlocks().size(); i++) {

        BufferSparseVolumeData::VolumeBlock& block = volume->VolumeBlocks()[i];

        if (block.isRaw) {
            if (block.format == BufferSparseVolumeData::FORMAT_FLOAT) {
                const float* buffer = reinterpret_cast<const float*>(block.rawData);
                if (block.components == 1) {
                    for (size_t i = 0; i < block.size[0] * block.size[1] * block.size[2]; i++) {
                        minVal[0] = std::min(minVal[0], (double)buffer[i]);
                        maxVal[0] = std::max(maxVal[0], (double)buffer[i]);
                    }
                } else if (block.components == 3) {
                    for (size_t i = 0; i < block.size[0] * block.size[1] * block.size[2]; i++) {
                        minVal[0] = std::min(minVal[0], (double)buffer[3*i+0]);
                        maxVal[0] = std::max(maxVal[0], (double)buffer[3*i+0]);
                        minVal[1] = std::min(minVal[1], (double)buffer[3*i+1]);
                        maxVal[1] = std::max(maxVal[1], (double)buffer[3*i+1]);
                        minVal[2] = std::min(minVal[2], (double)buffer[3*i+2]);
                        maxVal[2] = std::max(maxVal[2], (double)buffer[3*i+2]);
                    }
                } else {
                    // not supported.
                }

            } else if (block.format == BufferSparseVolumeData::FORMAT_DOUBLE) {
                const double* buffer = reinterpret_cast<const double*>(block.rawData);
                if (block.components == 1) {
                    for (size_t i = 0; i < block.size[0] * block.size[1] * block.size[2]; i++) {
                        minVal[0] = std::min(minVal[0], buffer[i]);
                        maxVal[0] = std::max(maxVal[0], buffer[i]);
                    }
                } else if (block.components == 3) {
                    for (size_t i = 0; i < block.size[0] * block.size[1] * block.size[2]; i++) {
                        minVal[0] = std::min(minVal[0], buffer[3*i+0]);
                        maxVal[0] = std::max(maxVal[0], buffer[3*i+0]);
                        minVal[1] = std::min(minVal[1], buffer[3*i+1]);
                        maxVal[1] = std::max(maxVal[1], buffer[3*i+1]);
                        minVal[2] = std::min(minVal[2], buffer[3*i+2]);
                        maxVal[2] = std::max(maxVal[2], buffer[3*i+2]);
                    }
                } else {
                    // not supported.
                }
            }

        } else {
            const float* buffer = static_cast<const float*>(block.volume->Buffer()->GetBuffer());
            const int temp_num[3] = {
                block.volume->Width(),
                block.volume->Height(),
                block.volume->Depth()
            };

            const int fnum = temp_num[0] + temp_num[1] + temp_num[2];
            if (temp_num[0] == 0 || temp_num[1] == 0 || temp_num[2] == 0 || fnum <= 0) {
                fprintf(stderr,"Volume data empty\n");
                return;
            }

            if (block.volume->Component() == 1) {
                for (size_t i = 0; i < temp_num[0] * temp_num[1] * temp_num[2]; i++) {
                    minVal[0] = std::min(minVal[0], (double)buffer[i]);
                    maxVal[0] = std::max(maxVal[0], (double)buffer[i]);
                }
            } else if (block.volume->Component() == 3) {
                for (size_t i = 0; i < temp_num[0] * temp_num[1] * temp_num[2]; i++) {
                    minVal[0] = std::min(minVal[0], (double)buffer[3*i+0]);
                    maxVal[0] = std::max(maxVal[0], (double)buffer[3*i+0]);
                    minVal[1] = std::min(minVal[1], (double)buffer[3*i+1]);
                    maxVal[1] = std::max(maxVal[1], (double)buffer[3*i+1]);
                    minVal[2] = std::min(minVal[2], (double)buffer[3*i+2]);
                    maxVal[2] = std::max(maxVal[2], (double)buffer[3*i+2]);
                }
            } else {
                fprintf(stderr,"# of components in the volume cell must be 1, 3.\n");
            }
        }
    }
}

// Support scalar or vector(3) only.
void HistogramContribute(Histogram* hist, int channel, BufferSparseVolumeData* volume)
{
    for (size_t i = 0; i < volume->VolumeBlocks().size(); i++) {

        BufferSparseVolumeData::VolumeBlock& block = volume->VolumeBlocks()[i];

        if (block.isRaw) {
            if (block.format == BufferSparseVolumeData::FORMAT_FLOAT) {
                const float* buffer = reinterpret_cast<const float*>(block.rawData);
                if (block.components == 1) {
                    for (size_t i = 0; i < block.size[0] * block.size[1] * block.size[2]; i++) {
                        hist->Contribute(buffer[i]);
                    }
                } else if (block.components == 3) {
                    for (size_t i = 0; i < block.size[0] * block.size[1] * block.size[2]; i++) {
                        hist->Contribute(buffer[3*i+channel]);
                    }
                } else {
                    // not supported.
                }

            } else if (block.format == BufferSparseVolumeData::FORMAT_DOUBLE) {
                const double* buffer = reinterpret_cast<const double*>(block.rawData);
                if (block.components == 1) {
                    for (size_t i = 0; i < block.size[0] * block.size[1] * block.size[2]; i++) {
                        hist->Contribute(buffer[i]);
                    }
                } else if (block.components == 3) {
                    for (size_t i = 0; i < block.size[0] * block.size[1] * block.size[2]; i++) {
                        hist->Contribute(buffer[3*i+channel]);
                    }
                } else {
                    // not supported.
                }
            }

        } else {
            const float* buffer = static_cast<const float*>(block.volume->Buffer()->GetBuffer());
            const int temp_num[3] = {
                block.volume->Width(),
                block.volume->Height(),
                block.volume->Depth()
            };

            const int fnum = temp_num[0] + temp_num[1] + temp_num[2];
            if (temp_num[0] == 0 || temp_num[1] == 0 || temp_num[2] == 0 || fnum <= 0) {
                return;
            }

            if (block.volume->Component() == 1) {
                for (size_t i = 0; i < temp_num[0] * temp_num[1] * temp_num[2]; i++) {
                    hist->Contribute(buffer[i]);
                }
            } else if (block.volume->Component() == 3) {
                for (size_t i = 0; i < temp_num[0] * temp_num[1] * temp_num[2]; i++) {
                    hist->Contribute(buffer[3*i+channel]);
                }
            } else {
                // not supported
            }
        }
    }
}

}   // namespace

/// コンストラクタ
SparseVolumeAnalyzer::SparseVolumeAnalyzer()
{
    Clear();
}

/// デストラクタ
SparseVolumeAnalyzer::~SparseVolumeAnalyzer()
{
}

/// メンバクリア
void SparseVolumeAnalyzer::Clear() {
    m_minVal[0] = m_minVal[1] = m_minVal[2] = 0.0;
    m_maxVal[0] = m_maxVal[1] = m_maxVal[2] = 0.0;
}

/**
 * 解析したボリュームデータX軸最小値
 * @return X軸最小値
 */
double SparseVolumeAnalyzer::MinX() {
    return m_minVal[0];
}

/**
 * 解析したボリュームデータX軸最小値
 * @return X軸最小値
 */
double SparseVolumeAnalyzer::MinY() {
    return m_minVal[1];
}

/**
 * 解析したボリュームデータX軸最小値
 * @return X軸最小値
 */
double SparseVolumeAnalyzer::MinZ() {
    return m_minVal[2];
}

/**
 * 解析したボリュームデータX軸最大値
 * @return X軸最大値
 */
double SparseVolumeAnalyzer::MaxX() {
    return m_maxVal[0];
}

/**
 * 解析したボリュームデータY軸最大値
 * @return Y軸最大値
 */
double SparseVolumeAnalyzer::MaxY() {
    return m_maxVal[1];
}

/**
 * 解析したボリュームデータZ軸最大値
 * @return Z軸最大値
 */
double SparseVolumeAnalyzer::MaxZ() {
    return m_maxVal[2];
}

/**
 * ヒストグラムを返す
 * @return ヒストグラム
 */
const std::vector<float>& SparseVolumeAnalyzer::GetHistgram() const
{
    return m_volHist[0];
}

/**
 * 最小最大を指定し, ヒストグラムを返す
 * @return ヒストグラム
 */
const std::vector<float> SparseVolumeAnalyzer::GetHistgramInRange(BufferSparseVolumeData *svolume, double minVal, double maxVal) const
{
    VolumeAnalyzerProc proc;
    std::vector<float> histgram[3];
    
    if (!svolume || svolume->GetType() != BufferData::TYPE_SPARSEVOLUME) {
        fprintf(stderr,"Invalid volume data\n");
        return histgram[0];
    }

    if (svolume->VolumeBlocks().empty()) {
        return histgram[0];
    }

    // 2. compute hisrogram with a specified range.
    int components = svolume->VolumeBlocks()[0].isRaw ? svolume->VolumeBlocks()[0].components : svolume->VolumeBlocks()[0].volume->Component();
    if (components == 1 || components == 3) {
        // OK
    } else {
        return histgram[0];
    }

    int numBins = 256;
    if (components == 1) {
        Histogram hist(numBins, minVal, maxVal);
        HistogramContribute(&hist, 0, svolume);
        hist.GetNormalizedHistogram(histgram[0]);
    } else {
        for (int c = 0; c < components; c++) {
            Histogram hist(numBins, minVal, maxVal);
            HistogramContribute(&hist, c, svolume);
            hist.GetNormalizedHistogram(histgram[c]);
        }
    }
        
    return histgram[0]; // @fixme { Only returns histgram of first volume component. }
}

/**
 * ボリュームモデル解析
 * @param model 解析対象VolumeModel
 * @retval true 成功
 * @retval false 失敗
 */
bool SparseVolumeAnalyzer::Execute(BufferSparseVolumeData *svolume)
{
    if (!svolume || svolume->GetType() != BufferData::TYPE_VOLUME) {
        fprintf(stderr,"Invalid volume data.");
        return false;
    }

    if (svolume->VolumeBlocks().empty()) {
        return false;
    }
   
    // 1. first find min/max
    m_minVal[0] = m_minVal[1] = m_minVal[2] = std::numeric_limits<double>::max();
    m_maxVal[0] = m_maxVal[1] = m_maxVal[2] = -std::numeric_limits<double>::max();

    FindMinMax(m_minVal, m_maxVal, svolume);

    // 2. compute hisrogram.
    int components = svolume->VolumeBlocks()[0].isRaw ? svolume->VolumeBlocks()[0].components : svolume->VolumeBlocks()[0].volume->Component();
    if (components == 1 || components == 3) {
        // OK
    } else {
        return false;
    }

    int numBins = 256;
    if (components == 1) {
        Histogram hist(numBins, m_minVal[0], m_maxVal[0]);
        HistogramContribute(&hist, 0, svolume);
        hist.GetNormalizedHistogram(m_volHist[0]);
    } else {
        for (int c = 0; c < components; c++) {
            Histogram hist(numBins, m_minVal[c], m_maxVal[c]);
            HistogramContribute(&hist, c, svolume);
            hist.GetNormalizedHistogram(m_volHist[c]);
        }
    }

    return true;
}

