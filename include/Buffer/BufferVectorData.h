/**
 * @file BufferVectorData.h
 * BufferVectorDataクラス
 */
#ifndef _BUFFERVECTORDATA_H_
#define _BUFFERVECTORDATA_H_

#include "Ref.h"
#include "BufferData.h"

class Vec3Buffer;
class Vec2Buffer;
class FloatBuffer;
class UintBuffer;

/**
 * BufferVectorDataクラス
 */
class BufferVectorData : public BufferData
{
private:
    class Impl;
    Impl* m_imp;
    
protected:
    BufferVectorData(BufferVectorData* inst);
    BufferVectorData();
    ~BufferVectorData();

public:
    static BufferVectorData* CreateInstance();
    
    void Create(int vertexnum);
    void Clear();
    void print();
    Vec3Buffer*        Position();
    const Vec3Buffer*  Position() const;
    Vec3Buffer*        Direction() ;
    const Vec3Buffer*  Direction() const;
};

#endif //_BUFFERVECTORDATA_H_

