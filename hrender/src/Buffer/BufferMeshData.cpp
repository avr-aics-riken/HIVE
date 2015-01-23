#include "BufferMeshData.h"

BufferMeshData::BufferMeshData(){}
BufferMeshData::~BufferMeshData(){}

void BufferMeshData::Clear()
{
	m_pos      = 0;
	m_normal   = 0;
	m_texcoord = 0;
	m_index    = 0;
	m_mat      = 0;
}

