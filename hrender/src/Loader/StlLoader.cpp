#include <math.h>

#include "StlLoader.h"
#include "SimpleSTLB.h"

STLLoader::STLLoader()
{
	mesh.Clear();
}

STLLoader::~STLLoader(){};

void STLLoader::Clear()
{
	mesh.Clear();
}

bool STLLoader::Load(const char* filename){
	Clear();
	SimpleSTLB obj;
	bool r = obj.Load(filename);
	mesh.m_pos = new Vec3Buffer();
	mesh.m_normal = new Vec3Buffer();
	mesh.m_mat = new FloatBuffer();
	mesh.m_index = new UintBuffer();
	mesh.m_texcoord = new Vec2Buffer();
	mesh.m_pos->Create(obj.GetVertexNum());
	memcpy(mesh.m_pos->GetBuffer(), obj.GetPositionBuffer(), sizeof(float)*3*mesh.m_pos->GetNum());
	mesh.m_normal->Create(obj.GetVertexNum());
	memcpy(mesh.m_normal->GetBuffer(), obj.GetNormalBuffer(), sizeof(float)*3*mesh.m_normal->GetNum());
	mesh.m_mat->Create(obj.GetVertexNum());
	memset(mesh.m_mat->GetBuffer(), 0, sizeof(float)*mesh.m_mat->GetNum());
	mesh.m_index->Create(obj.GetIndexNum());
	memcpy(mesh.m_index->GetBuffer(), obj.GetIndex(), sizeof(unsigned int)*mesh.m_index->GetNum());

	printf("%d\n", mesh.m_pos.Get()->GetNum()     );
	printf("%d\n", mesh.m_normal.Get()->GetNum()  );
	printf("%d\n", mesh.m_mat.Get()->GetNum()     );
	printf("%d\n", mesh.m_index.Get()->GetNum()   );
	return r;
}

BufferMeshData *STLLoader::MeshData()
{
	return &mesh;
}

