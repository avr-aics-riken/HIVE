/**
 * @file BufferImageData.h
 * BufferImageDataクラス
 */
#ifndef _BUFFERIMAGEDATA_H_
#define _BUFFERIMAGEDATA_H_

#include "Ref.h"
#include "BufferData.h"

class FloatBuffer;
class ByteBuffer;

/**
 * BufferImageDataクラス
 */
class BufferImageData : public BufferData
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

protected:
    BufferImageData();
    BufferImageData(BufferImageData* inst);
    BufferImageData(const BufferImageData* inst);
    ~BufferImageData();
    
public:
    static BufferImageData* CreateInstance();
    
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
