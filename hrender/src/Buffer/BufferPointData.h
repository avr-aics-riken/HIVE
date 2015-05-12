/**
 * @file BufferPointData.h
 * BufferPointDataクラス
 */
#ifndef _BUFFERPOINTDATA_H_
#define _BUFFERPOINTDATA_H_

#include "Ref.h"
#include "BufferData.h"

class Vec3Buffer;
class FloatBuffer;

/**
 * BufferPointDataクラス
 */
class BufferPointData : public BufferData
{
private:
    class Impl;
    Impl* m_imp;
    
public:
    BufferPointData();
    BufferPointData(BufferPointData* inst);
    ~BufferPointData();
    void Create(int vertexnum);
    void Clear();
    Vec3Buffer*  Position();
    FloatBuffer* Radius();
    FloatBuffer* Material();
    Vec3Buffer*  Position() const;
    FloatBuffer* Radius()   const;
    FloatBuffer* Material() const;
};

#endif //_BUFFERPOINTDATA_H_

