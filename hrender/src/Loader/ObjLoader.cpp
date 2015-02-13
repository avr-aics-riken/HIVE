#include "ObjLoader.h"
#include "SimpleObj.h"
#include "BufferMeshData.h"
#include "BufferPointData.h"

OBJLoader::OBJLoader()
{
    m_mesh  = 0;
    m_point = 0;
    m_obj   = 0;
}

OBJLoader::~OBJLoader()
{
    delete m_obj;
}

void OBJLoader::Clear()
{
    delete m_obj;
    m_obj   = 0;
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
    float* pp = pos->GetBuffer();
    memcpy(pp, obj.GetPositionBuffer(), sizeof(float)*3*(obj.GetVertexNum()));
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
    float* pp = pos->GetBuffer();
    memcpy(pp, obj.GetPositionBuffer(), sizeof(float)*3*(obj.GetVertexNum()));
    float* rad = radius->GetBuffer();
    for (int i = 0; i < vnum; ++i) {
        rad[i] = 1.0f;
    }
    memset(mat->GetBuffer(), 0, sizeof(float) * mat->GetNum());
    return point;
}

bool OBJLoader::Load(const char* filename){
	Clear();
    
	m_obj = new SimpleObj();
	bool r = m_obj->Load(filename);
    if (!r) {
        delete m_obj;
        return false;
    }
    m_mesh  = 0;
    m_point = 0;
    
	return r;
}

BufferMeshData *OBJLoader::MeshData()
{
    if (!m_mesh)
        m_mesh  = createMeshData(*m_obj);
	return m_mesh;
}

BufferPointData *OBJLoader::PointData()
{
    if (!m_point)
        m_point = createPointData(*m_obj);

    return m_point;
}


