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

void BufferMeshData::Create(int vertexnum, int indexnum)
{
    m_pos      = new Vec3Buffer();
    m_normal   = new Vec3Buffer();
    m_texcoord = new Vec2Buffer();
    m_mat      = new FloatBuffer();
    m_index    = new UintBuffer();
    m_pos->Create(vertexnum);
    m_normal->Create(vertexnum);
    m_texcoord->Create(vertexnum);
    m_mat->Create(vertexnum);
    m_index->Create(indexnum);
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

