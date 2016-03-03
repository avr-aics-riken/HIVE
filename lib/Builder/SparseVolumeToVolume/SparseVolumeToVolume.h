/**
 * @file SparseVolumeToVolume.h
 * VolumeDataからVectorメッシュデータを作成するモジュール
 */
#ifndef _SPARSEVOLUMETOVOLUME_H_
#define _SPARSEVOLUMETOVOLUME_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>
#include "BufferSparseVolumeData.h"
#include "BufferVolumeData.h"

/**
 * Create VolumeData from SparseVolumeData.
 */
class SparseVolumeToVolume : public RefCount
{
private:
    BufferVolumeData* m_volume;

public:
    SparseVolumeToVolume();
    int Create(BufferSparseVolumeData *sparseVolume, float resamplingRate);
    BufferVolumeData*  VolumeData();

};

#endif //_SPARSEVOLUMETOVOLUME_H_

