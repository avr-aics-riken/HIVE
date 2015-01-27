#include "BufferMeshData.h"

BufferMeshData::BufferMeshData()
{
	printf("Buffer Mesh Data Cons\n");
}

BufferMeshData::~BufferMeshData()
{
	printf("Buffer Mesh Data Destruct\n");
}

void BufferMeshData::Clear()
{
	m_pos      = 0;
	m_normal   = 0;
	m_texcoord = 0;
	m_index    = 0;
	m_mat      = 0;
}

void BufferMeshData::print()
{
/*	printf("%s:Position() %08p\n", __func__, m_pos.Get()      );
	printf("%s:Normal()   %08X\n", __func__, m_normal.Get()   );
	printf("%s:Texcoord() %08X\n", __func__, m_texcoord.Get() );
	printf("%s:Index()    %08X\n", __func__, m_index.Get()    );
	printf("%s:Material() %08X\n", __func__, m_mat.Get()      );*/
}

Vec3Buffer  *BufferMeshData::Position() { return m_pos;      }
Vec3Buffer  *BufferMeshData::Normal()   { return m_normal;   }
Vec2Buffer  *BufferMeshData::Texcoord() { return m_texcoord; }
UintBuffer  *BufferMeshData::Index()    { return m_index;    }
FloatBuffer *BufferMeshData::Material() { return m_mat;      }

