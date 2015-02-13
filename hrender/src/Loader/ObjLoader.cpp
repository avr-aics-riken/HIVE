#include "ObjLoader.h"
#include "SimpleObj.h"
#include "BufferMeshData.h"
#include "BufferPointData.h"

OBJLoader::OBJLoader()
{
    m_mesh  = 0;
    m_point = 0;
}

OBJLoader::~OBJLoader()
{
}

void OBJLoader::Clear()
{
    m_mesh  = 0;
    m_point = 0;
}

BufferMeshData* OBJLoader::createMeshData(const SimpleObj& obj) const
{
    BufferMeshData* mesh  = new BufferMeshData();
    mesh->Create(obj.GetVertexNum(), obj.GetIndexNum());
    Vec3Buffer* pos      = mesh->Position();
    Vec3Buffer* normal   = mesh->Normal();
    FloatBuffer* mat     = mesh->Material();
    UintBuffer* index    = mesh->Index();
    Vec2Buffer* texcoord = mesh->Texcoord();
    
    pos->Create(obj.GetVertexNum());
    const float* pp = pos->GetBuffer();
    memcpy(&pp, obj.GetPositionBuffer(), sizeof(float)*3*(obj.GetVertexNum()));
    normal->Create(obj.GetVertexNum());
    memcpy(normal->GetBuffer(), obj.GetNormalBuffer(), sizeof(float)*3*obj.GetVertexNum());
    const float* objuv = obj.GetUVBuffer();
    if (objuv) {
        texcoord->Create(obj.GetVertexNum());
        float* uv = texcoord->GetBuffer();
        memcpy(uv, objuv, sizeof(float)*2*(obj.GetVertexNum()));
    }
    mat->Create(obj.GetVertexNum());
    memset(mat->GetBuffer(), 0, sizeof(float) * mat->GetNum());
    index->Create(obj.GetIndexNum());
    memcpy(index->GetBuffer(), obj.GetIndex(), sizeof(unsigned int) * index->GetNum());

    return mesh;
}

BufferPointData* OBJLoader::createPointData(const SimpleObj& obj) const
{
    BufferPointData* point = new BufferPointData();
    
    point->Create(obj.GetVertexNum());
    Vec3Buffer*  pos     = point->Position();
    FloatBuffer* mat     = point->Material();
    FloatBuffer* radius  = point->Radius();

    const int vnum = obj.GetVertexNum();
    pos->Create(vnum);
    const float* pp = pos->GetBuffer();
    memcpy(&pp, obj.GetPositionBuffer(), sizeof(float)*3*(obj.GetVertexNum()));
    radius->Create(vnum);
    float* rad = radius->GetBuffer();
    for (int i = 0; i < vnum; ++i) {
        rad[i] = 1.0f;
    }
    mat->Create(obj.GetVertexNum());
    memset(mat->GetBuffer(), 0, sizeof(float) * mat->GetNum());
    return point;
}

bool OBJLoader::Load(const char* filename){
	Clear();
    
	SimpleObj obj;
	bool r = obj.Load(filename);
    if (!r)
        return false;
    
    createMeshData(obj);
    createPointData(obj);
    
	return r;
}

BufferMeshData *OBJLoader::MeshData()
{
	return m_mesh;
}

BufferPointData *OBJLoader::PointData()
{
    return m_point;
}


