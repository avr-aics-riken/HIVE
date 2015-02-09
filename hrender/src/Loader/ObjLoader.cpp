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
    if (!r)
        return false;
    
    mesh.Create(obj.GetVertexNum(), obj.GetIndexNum());
    Vec3Buffer* pos      = mesh.Position();
    Vec3Buffer* normal   = mesh.Normal();
    FloatBuffer* mat     = mesh.Material();
    UintBuffer* index    = mesh.Index();
    Vec2Buffer* texcoord = mesh.Texcoord();


	pos->Create(obj.GetVertexNum());
	float* pp = pos->GetBuffer();
	memcpy(pp, obj.GetPositionBuffer(), sizeof(float)*3*(obj.GetVertexNum()));
	normal->Create(obj.GetVertexNum());
	memcpy(normal->GetBuffer(), obj.GetNormalBuffer(), sizeof(float)*3*obj.GetVertexNum());
	float* objuv = obj.GetUVBuffer();
	if (objuv) {
		texcoord->Create(obj.GetVertexNum());
		float* uv = texcoord->GetBuffer();
		memcpy(uv, objuv, sizeof(float)*2*(obj.GetVertexNum()));
	}
	mat->Create(obj.GetVertexNum());
	memset(mat->GetBuffer(), 0, sizeof(float) * mat->GetNum());
	index->Create(obj.GetIndexNum());
	memcpy(index->GetBuffer(), obj.GetIndex(), sizeof(unsigned int) * index->GetNum());
    

	
	return r;
}

BufferMeshData *OBJLoader::MeshData()
{
	return &mesh;
}


