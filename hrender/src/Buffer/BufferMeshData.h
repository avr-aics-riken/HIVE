/**
 * @file BufferMeshData.h
 * BufferMeshDataクラス
 */
#ifndef _BUFFERMESHDATA_H_
#define _BUFFERMESHDATA_H_

#include "Ref.h"

class Vec3Buffer;
class Vec2Buffer;
class FloatBuffer;
class UintBuffer;

/**
 * BufferMeshDataクラス
 */
class BufferMeshData : public RefCount
{
private:
    class Impl;
    Impl* m_imp;
    
public:
    BufferMeshData(BufferMeshData* inst);
	BufferMeshData();
	~BufferMeshData();
    void Create(int vertexnum, int indexnum);
	void Clear();
	void print();
	Vec3Buffer*  Position() ;
	Vec3Buffer*  Normal()   ;
	Vec2Buffer*  Texcoord() ;
	UintBuffer*  Index()    ;
	FloatBuffer* Material() ;
    Vec3Buffer*  Position() const;
    Vec3Buffer*  Normal()   const;
    Vec2Buffer*  Texcoord() const;
    UintBuffer*  Index()    const;
    FloatBuffer* Material() const;

};

#endif //_BUFFERMESHDATA_H_

