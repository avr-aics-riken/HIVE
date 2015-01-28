#ifndef _BUFFERVOLUMEDATA_H_
#define _BUFFERVOLUMEDATA_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>

class BufferVolumeData : public RefCount
{
public:
    int m_dim[3];
    int m_comp;
    RefPtr<FloatBuffer>  m_buffer;
    BufferVolumeData();
    BufferVolumeData(BufferVolumeData* inst);
    ~BufferVolumeData();
    void Clear();
    void print();
    int Width() ;
    int Height();
    int Depth() ;
    int Component();
    FloatBuffer *Buffer();
};

#endif //_BUFFERVOLUMEDATA_H_

