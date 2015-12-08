/**
 * @file BufferSparseVolumeData.h
 * BufferSparseVolumeDataクラス
 */
#ifndef _BUFFERSPARSEVOLUMEDATA_H_
#define _BUFFERSPARSEVOLUMEDATA_H_

#include "Ref.h"
#include "BufferData.h"
#include <vector>

class BufferVolumeData;

/**
 * BufferSparseVolumeDataクラス
 */
class BufferSparseVolumeData : public BufferData
{
private:
    class Impl;
    Impl* m_imp;

public:
    typedef struct {
        int offset[3];
        BufferVolumeData* volume;
    } VolumeBlock;

    BufferSparseVolumeData();
    BufferSparseVolumeData(BufferSparseVolumeData* inst);
    ~BufferSparseVolumeData();
    void Create(int w, int h, int d, int component);
    void AddVolume(int offset_x, int offset_y, int offset_z,
                   BufferVolumeData* vol);
    void Clear();
    void print();
    const int Width() const;
    const int Height() const;
    const int Depth() const;
    const int Component() const;
    const std::vector<VolumeBlock>& VolumeBlocks() const;
    std::vector<VolumeBlock>& VolumeBlocks();

    // Fetch voxel data. `ret` pointer must have enough storage space to store voxel data,
    // larger than # of compoents in this volume data.
    // Assume x, y and z are in the range of unit space: i,e. [0, 1)^3
    void Sample(float* ret, float x, float y, float z);

    // Build spatial acceraltion for SparseVolume.
    // Only required if you want to fetch voxel data through Sample() function.
    bool Build();

    // Return whether Build() has been called already.
    bool IsBuilt() const;

};

#endif //_BUFFERSPARSEVOLUMEDATA_H_
