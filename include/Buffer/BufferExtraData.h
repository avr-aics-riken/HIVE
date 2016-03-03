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

protected:
    BufferExtraData();
    BufferExtraData(const BufferExtraData* inst);
    ~BufferExtraData();

public:
    static BufferExtraData* CreateInstance();
    
    void Create(const char* dataType, int num);
    const char* GetDataType() const;
    void Clear();
    int GetNum() const;
    
    FloatBuffer* Float();
    UintBuffer* Uint();
    Vec4Buffer* Vec4();
    Vec3Buffer* Vec3();
    Vec2Buffer* Vec2();
    
    const FloatBuffer* Float() const;
    const UintBuffer*  Uint()  const;
    const Vec4Buffer*  Vec4()  const;
    const Vec3Buffer*  Vec3()  const;
    const Vec2Buffer*  Vec2()  const;

};

#endif //_BUFFEREXTRADATA_H_
