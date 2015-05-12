/**
 * @file BufferExtraData.h
 * BufferExtraDataクラス
 */
#ifndef _BUFFEREXTRADATA_H_
#define _BUFFEREXTRADATA_H_

#include "Ref.h"
#include "BufferData.h"

class FloatBuffer;
class UintBuffer;
class Vec4Buffer;
class Vec3Buffer;
class Vec2Buffer;

/**
 * BufferExtraDataクラス
 */
class BufferExtraData : public BufferData
{
private:
    class Impl;
    Impl* m_imp;

public:
    BufferExtraData();
    BufferExtraData(const BufferExtraData* inst);
    ~BufferExtraData();
    void Create(const char* dataType, int num);
    const char* GetDataType() const;
    void Clear();
    int GetNum() const;
    FloatBuffer* FloatBuffer();
    UintBuffer* UintBuffer();
    Vec4Buffer* Vec4Buffer();
    Vec3Buffer* Vec3Buffer();
    Vec2Buffer* Vec2Buffer();
    
    const class FloatBuffer* FloatBuffer() const;
    const class UintBuffer* UintBuffer() const;
    const class Vec4Buffer* Vec4Buffer() const;
    const class Vec3Buffer* Vec3Buffer() const;
    const class Vec2Buffer* Vec2Buffer() const;

};

#endif //_BUFFEREXTRADATA_H_
