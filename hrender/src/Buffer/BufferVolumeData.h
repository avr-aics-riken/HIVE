#ifndef _BUFFERVOLUMEDATA_H_
#define _BUFFERVOLUMEDATA_H_

#include "Ref.h"

class FloatBuffer;

class BufferVolumeData : public RefCount
{
private:
    class Impl;
    Impl* m_imp;
    
public:
    BufferVolumeData();
    BufferVolumeData(BufferVolumeData* inst);
    ~BufferVolumeData();
    void Create(int w, int h, int d, int component);
    FloatBuffer* SpacingX();
    FloatBuffer* SpacingY();
    FloatBuffer* SpacingZ();
    bool &NonUniform();
    void Clear();
    void print();
    int Width() ;
    int Height();
    int Depth() ;
    int Component();
    FloatBuffer *Buffer();
};

#endif //_BUFFERVOLUMEDATA_H_

