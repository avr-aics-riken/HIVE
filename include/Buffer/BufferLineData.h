/**
 * @file BufferLineData.h
 * BufferLineDataクラス
 */
#ifndef _BUFFERLINEDATA_H_
#define _BUFFERLINEDATA_H_

#include "Ref.h"
#include "BufferData.h"

class Vec3Buffer;
class FloatBuffer;
class UintBuffer;
/**
 * BufferLineDataクラス
 */
class BufferLineData : public BufferData
{
private:
    class Impl;
    Impl* m_imp;
    
protected:
    BufferLineData();
    BufferLineData(BufferLineData* inst);
    ~BufferLineData();

public:
    static BufferLineData* CreateInstance();
    
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

