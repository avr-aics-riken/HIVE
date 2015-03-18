/**
 * @file BufferLineData.h
 * BufferLineDataクラス
 */
#ifndef _BUFFERLINEDATA_H_
#define _BUFFERLINEDATA_H_

#include "Ref.h"

class Vec3Buffer;
class FloatBuffer;
class UintBuffer;
/**
 * BufferLineDataクラス
 */
class BufferLineData : public RefCount
{
private:
    class Impl;
    Impl* m_imp;
    
public:
    BufferLineData();
    BufferLineData(BufferLineData* inst);
    ~BufferLineData();
    void Create(int vertexnum, int indexnum, bool useRadius = false);
    void Clear();
    Vec3Buffer*  Position();
    FloatBuffer* Radius();
    FloatBuffer* Material();
    UintBuffer* Index();
    Vec3Buffer*  Position() const;
    FloatBuffer* Radius()   const;
    FloatBuffer* Material() const;
    UintBuffer*  Index()    const;
};

#endif //_BUFFERLINEDATA_H_

