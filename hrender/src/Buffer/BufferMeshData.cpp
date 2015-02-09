#include "BufferMeshData.h"

BufferMeshData::BufferMeshData()
{
}

BufferMeshData::~BufferMeshData()
{
}

BufferMeshData::BufferMeshData(BufferMeshData* inst)
{
	this->m_pos      = inst->m_pos     ;
	this->m_normal   = inst->m_normal  ;
	this->m_texcoord = inst->m_texcoord;
	this->m_index    = inst->m_index   ;
	this->m_mat      = inst->m_mat     ;
}

void BufferMeshData::Create(Vec3Buffer* pos, Vec3Buffer* normal, Vec2Buffer* texcoord, UintBuffer* index, FloatBuffer* mat)
{
    m_pos      = pos;
    m_normal   = normal;
    m_texcoord = texcoord;
    m_index    = index;
    m_mat      = mat;
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
/*	printf("Position() %08p\n", m_pos.Get()      );
	printf("Normal()   %08X\n", m_normal.Get()   );
	printf("Texcoord() %08X\n", m_texcoord.Get() );
	printf("Index()    %08X\n", m_index.Get()    );
	printf("Material() %08X\n", m_mat.Get()      );*/
}

Vec3Buffer  *BufferMeshData::Position() { return m_pos;      }
Vec3Buffer  *BufferMeshData::Normal()   { return m_normal;   }
Vec2Buffer  *BufferMeshData::Texcoord() { return m_texcoord; }
UintBuffer  *BufferMeshData::Index()    { return m_index;    }
FloatBuffer *BufferMeshData::Material() { return m_mat;      }

