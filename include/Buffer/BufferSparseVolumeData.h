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
    typedef enum {
      FORMAT_FLOAT,
      FORMAT_DOUBLE,
    } VolumeBlockFormat;

    typedef struct {
        int level;                ///< LoD level
        int offset[3];            ///< Offset in world space(Origin)
        int extent[3];            ///< Extent in world space

        bool isRaw;               ///< RAW flag(true = use rawData, false = use BufferVolumeData)

        int size[3];              ///< Cell size(actual voxel size) (isRaw only)
        int components;           ///< # of components in the voxel(isRaw only).
        VolumeBlockFormat format; ///< Data format(isRaw only)
        const unsigned char *rawData;   ///< Pointer to volume data(isRaw only)

        BufferVolumeData* volume; ///< NULL when isRaw = true
    } VolumeBlock;

protected:
    BufferSparseVolumeData();
    BufferSparseVolumeData(BufferSparseVolumeData* inst);
    ~BufferSparseVolumeData();
    
public:
    static BufferSparseVolumeData* CreateInstance();
    
    void Create();
    void AddVolumeBlock(int level, int offset_x, int offset_y, int offset_z,
                   int extent_x, int extent_y, int extent_z,
                   BufferVolumeData* vol);
    // In-Situ version. Memory of volume block is maintained by the external app,
    // and don't create volume object in HIVE.
    void AddRAWVolumeBlock(int level, int offset_x, int offset_y, int offset_z,
                   int extent_x, int extent_y, int extent_z,
                   int size_x, int size_y, int size_z,
                   int components, VolumeBlockFormat format, const void *data);
    void Clear();
    void print();

    // Width(), Height() and Depth() are valid after adding all volume blocks.
    const int Width() const;
    const int Height() const;
    const int Depth() const;
    //const int CellWidth() const;
    //const int CellHeight() const;
    //const int CellDepth() const;
    //const int Component() const;
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
