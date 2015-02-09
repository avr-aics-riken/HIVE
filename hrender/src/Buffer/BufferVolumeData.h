#ifndef _BUFFERVOLUMEDATA_H_
#define _BUFFERVOLUMEDATA_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>

class BufferVolumeData : public RefCount
{
private:
    int m_dim[3];
    int m_comp;
    RefPtr<FloatBuffer>  m_buffer;
    RefPtr<FloatBuffer>  m_spacingX;
    RefPtr<FloatBuffer>  m_spacingY;
    RefPtr<FloatBuffer>  m_spacingZ;
public:
    BufferVolumeData();
    BufferVolumeData(BufferVolumeData* inst);
    ~BufferVolumeData();
    void Create(int w, int h, int d, int component);
    FloatBuffer* SpacingX();
    FloatBuffer* SpacingY();
    FloatBuffer* SpacingZ();
    void Clear();
    void print();
    int Width() ;
    int Height();
    int Depth() ;
    int Component();
    FloatBuffer *Buffer();
};

#endif //_BUFFERVOLUMEDATA_H_

