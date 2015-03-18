/**
 * @file BufferTetraData.h
 * BufferTetraDataクラス
 */
#ifndef _BUFFERTETRADATA_H_
#define _BUFFERTETRADATA_H_

#include "Ref.h"

class Vec3Buffer;
class Vec2Buffer;
class FloatBuffer;
class UintBuffer;

/**
 * BufferTetraDataクラス
 */
class BufferTetraData : public RefCount
{
private:
    class Impl;
    Impl* m_imp;
    
public:
    BufferTetraData(BufferTetraData* inst);
    BufferTetraData();
    ~BufferTetraData();
    void Create(int vertexnum, int indexnum);
    void Clear();
    void print();
    Vec3Buffer*  Position() ;
    UintBuffer*  Index()    ;
    Vec3Buffer*  Position() const;
    UintBuffer*  Index()    const;

};

#endif //_BUFFERTETRADATA_H_

