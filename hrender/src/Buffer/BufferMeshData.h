#ifndef _BUFFERMESHDATA_H_
#define _BUFFERMESHDATA_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>

class BufferMeshData : public RefCount
{
private:
	RefPtr<Vec3Buffer >  m_pos;
	RefPtr<Vec3Buffer >  m_normal;
	RefPtr<Vec2Buffer >  m_texcoord;
	RefPtr<UintBuffer >  m_index;
	RefPtr<FloatBuffer>  m_mat;
public:
    BufferMeshData(BufferMeshData* inst);
	BufferMeshData();
	~BufferMeshData();
    void Create(Vec3Buffer* pos, Vec3Buffer* normal, Vec2Buffer* texcoord, UintBuffer* index, FloatBuffer* mat);
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

