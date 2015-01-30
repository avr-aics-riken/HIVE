#ifndef _BUFFERMESHDATA_H_
#define _BUFFERMESHDATA_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>

class BufferMeshData : public RefCount
{
public:
	BufferMeshData(BufferMeshData* inst);
	RefPtr<Vec3Buffer >  m_pos;
	RefPtr<Vec3Buffer >  m_normal;
	RefPtr<Vec2Buffer >  m_texcoord;
	RefPtr<UintBuffer >  m_index;
	RefPtr<FloatBuffer>  m_mat;
	BufferMeshData();
	~BufferMeshData();
	void Clear();
	void print();
	Vec3Buffer*  Position() ;
	Vec3Buffer*  Normal()   ;
	Vec2Buffer*  Texcoord() ;
	UintBuffer*  Index()    ;
	FloatBuffer* Material() ;
};

#endif //_BUFFERMESHDATA_H_

