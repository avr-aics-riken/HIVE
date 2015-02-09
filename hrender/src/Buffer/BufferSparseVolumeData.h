#ifndef _BUFFERSPARSEVOLUMEDATA_H_
#define _BUFFERSPARSEVOLUMEDATA_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"
#include <vector>

class BufferSparseVolumeData : public RefCount
{
public:
    typedef struct {
        int offset[3];
        BufferVolumeData* volume;
    } VolumeBlock;

private:
    int m_dim[3];
    int m_comp;
    std::vector<VolumeBlock> m_volumes;
    
public:
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

