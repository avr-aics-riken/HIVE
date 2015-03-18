/**
 * @file BufferVolumeData.cpp
 * BufferVolumeDataクラス
 */
#ifndef _BUFFERVOLUMEDATA_H_
#define _BUFFERVOLUMEDATA_H_

#include "Ref.h"

class FloatBuffer;
/**
 * BufferVolumeDataクラス
 */
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
    bool NonUniform();
    void Clear();
    void print();
    int Width() const;
    int Height() const;
    int Depth() const;
    int Component() const;
    FloatBuffer *Buffer() const;

    // Fetch voxel data. `ret` pointer must have enough storage space to store voxel data,
    // larger than # of compoents in this volume data.
    // Assume x, y and z are in the range of unit space: i,e. [0, 1)^3
    void Sample(float* ret, float x, float y, float z);
};

#endif //_BUFFERVOLUMEDATA_H_

