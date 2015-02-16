#ifndef _BUFFERSPARSEVOLUMEDATA_H_
#define _BUFFERSPARSEVOLUMEDATA_H_

#include "Ref.h"
#include <vector>

class BufferVolumeData;

class BufferSparseVolumeData : public RefCount
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
    int Width() ;
    int Height();
    int Depth() ;
    int Component();
    const std::vector<VolumeBlock>& Buffers() const;
    std::vector<VolumeBlock>& Buffers();
};

#endif //_BUFFERSPARSEVOLUMEDATA_H_

