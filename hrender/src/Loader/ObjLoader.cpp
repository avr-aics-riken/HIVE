#include "ObjLoader.h"
#include "SimpleObj.h"

OBJLoader::OBJLoader(){}
OBJLoader::~OBJLoader(){};

void OBJLoader::Clear()
{

	mesh.Clear();
}

bool OBJLoader::Load(const char* filename){
	Clear();
	SimpleObj obj;
	bool r = obj.Load(filename);
	mesh.m_pos = new Vec3Buffer();
	mesh.m_normal = new Vec3Buffer();
	mesh.m_mat = new FloatBuffer();
	mesh.m_index = new UintBuffer();
	mesh.m_texcoord = new Vec2Buffer();
	mesh.m_pos->Create(obj.GetVertexNum());
	float* pp = mesh.m_pos->GetBuffer();
	memcpy(pp, obj.GetPositionBuffer(), sizeof(float)*3*(obj.GetVertexNum()));
	mesh.m_normal->Create(obj.GetVertexNum());
	memcpy(mesh.m_normal->GetBuffer(), obj.GetNormalBuffer(), sizeof(float)*3*obj.GetVertexNum());
	float* objuv = obj.GetUVBuffer();
	if (objuv) {
		mesh.m_texcoord->Create(obj.GetVertexNum());
		float* uv = mesh.m_texcoord->GetBuffer();
		memcpy(uv, objuv, sizeof(float)*2*(obj.GetVertexNum()));
	}
	mesh.m_mat->Create(obj.GetVertexNum());
	memset(mesh.m_mat->GetBuffer(), 0, sizeof(float)*mesh.m_mat->GetNum());
	mesh.m_index->Create(obj.GetIndexNum());
	memcpy(mesh.m_index->GetBuffer(), obj.GetIndex(), sizeof(unsigned int)*mesh.m_index->GetNum());
	
	return r;
}

BufferMeshData OBJLoader::MeshData()
{
	return mesh;
}


