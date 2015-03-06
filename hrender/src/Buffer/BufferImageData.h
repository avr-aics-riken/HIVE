#ifndef _BUFFERIMAGEDATA_H_
#define _BUFFERIMAGEDATA_H_

#include "Ref.h"

class FloatBuffer;
class ByteBuffer;

class BufferImageData : public RefCount
{
private:
    class Impl;
    Impl* m_imp;

public:
    enum FORMAT {
        INVALID,
        RGB8,
        RGBA8,
        R32F,
        RGBA32F,
    };
    
    BufferImageData();
    BufferImageData(BufferImageData* inst);
    ~BufferImageData();
    void Create(FORMAT type, int w, int h);
    void Clear();
    int Width() const;
    int Height() const;
    FORMAT Format() const;
    int Bytes() const;
    FloatBuffer *FloatImageBuffer();
    FloatBuffer *FloatImageBuffer() const;
    ByteBuffer *ImageBuffer();
    ByteBuffer *ImageBuffer() const;
    float Gamma() const;
    float Exposure() const;
    void SetGamma(float gamma);
    void SetExposure(float exposure);
};

#endif //_BUFFERIMAGEDATA_H_
