#include "ObjLoader.h"
#include "SimpleObj.h"
#include "BufferMeshData.h"
#include "BufferPointData.h"
#include "BufferLineData.h"
#include "Buffer.h"

OBJLoader::OBJLoader()
{
    m_mesh  = 0;
    m_point = 0;
    m_obj   = 0;
    m_line  = 0;
}

OBJLoader::~OBJLoader()
{
    Clear();
}

void OBJLoader::Clear()
{
    delete m_obj;
    m_obj   = 0;
    m_mesh  = 0;
    m_point = 0;
    m_line  = 0;
}

BufferMeshData* OBJLoader::createMeshData(const SimpleObj* obj) const
{
    BufferMeshData* mesh  = new BufferMeshData();
    if (!obj) {
        return mesh;
    }
    mesh->Create(obj->GetVertexNum(), obj->GetIndexNum());
    Vec3Buffer* pos      = mesh->Position();
    Vec3Buffer* normal   = mesh->Normal();
    FloatBuffer* mat     = mesh->Material();
    UintBuffer* index    = mesh->Index();
    Vec2Buffer* texcoord = mesh->Texcoord();
    
    pos->Create(obj->GetVertexNum());
    float* pp = pos->GetBuffer();
    memcpy(pp, obj->GetPositionBuffer(), sizeof(float)*3*(obj->GetVertexNum()));
    normal->Create(obj->GetVertexNum());
    memcpy(normal->GetBuffer(), obj->GetNormalBuffer(), sizeof(float)*3*obj->GetVertexNum());
    const float* objuv = obj->GetUVBuffer();
    if (objuv) {
        texcoord->Create(obj->GetVertexNum());
        float* uv = texcoord->GetBuffer();
        memcpy(uv, objuv, sizeof(float)*2*(obj->GetVertexNum()));
    }
    mat->Create(obj->GetVertexNum());
    memset(mat->GetBuffer(), 0, sizeof(float) * mat->GetNum());
    index->Create(obj->GetIndexNum());
    memcpy(index->GetBuffer(), obj->GetIndex(), sizeof(unsigned int) * index->GetNum());

    return mesh;
}

BufferPointData* OBJLoader::createPointData(const SimpleObj* obj) const
{
    BufferPointData* point = new BufferPointData();
    if (!obj) {
        return point;
    }

    point->Create(obj->GetVertexNum());
    Vec3Buffer*  pos     = point->Position();
    FloatBuffer* mat     = point->Material();
    FloatBuffer* radius  = point->Radius();

    const int vnum = obj->GetVertexNum();
    float* pp = pos->GetBuffer();
    memcpy(pp, obj->GetPositionBuffer(), sizeof(float)*3*(obj->GetVertexNum()));
    float* rad = radius->GetBuffer();
    for (int i = 0; i < vnum; ++i) {
        rad[i] = 1.0f;
    }
    memset(mat->GetBuffer(), 0, sizeof(float) * mat->GetNum());
    return point;
}

BufferLineData* OBJLoader::createLineData(const SimpleObj* obj) const
{
    BufferLineData* line = new BufferLineData();
    if (!line) {
        return line;
    }

    // calc edge num
    std::map<std::pair<unsigned int, unsigned int>, int> lines;
    const unsigned int n = obj->GetIndexNum()/3;
    for (unsigned int i = 0; i < n; ++i) {
        unsigned int idx0 = obj->GetIndex()[3 * i + 0];
        unsigned int idx1 = obj->GetIndex()[3 * i + 1];
        unsigned int idx2 = obj->GetIndex()[3 * i + 2];
        const unsigned int l0 = idx0 < idx1 ? idx0 : idx1;
        const unsigned int l1 = idx0 > idx1 ? idx0 : idx1;
        const unsigned int l2 = idx0 < idx2 ? idx0 : idx2;
        const unsigned int l3 = idx0 > idx2 ? idx0 : idx2;
        const unsigned int l4 = idx1 < idx2 ? idx1 : idx2;
        const unsigned int l5 = idx1 > idx2 ? idx1 : idx2;
        lines[std::make_pair(l0, l1)] = 0;
        lines[std::make_pair(l2, l3)] = 0;
        lines[std::make_pair(l4, l5)] = 0;
    }
    
    const int indexnum = lines.size() * 2;
    line->Create(obj->GetVertexNum(), indexnum);
    Vec3Buffer*  pos     = line->Position();
    FloatBuffer* mat     = line->Material();
    FloatBuffer* radius  = line->Radius();
    UintBuffer*  idx     = line->Index();
    
    const int vnum = obj->GetVertexNum();
    float* pp = pos->GetBuffer();
    memcpy(pp, obj->GetPositionBuffer(), sizeof(float)*3*(obj->GetVertexNum()));
    float* rad = radius->GetBuffer();
    if (rad) {
        for (int i = 0; i < vnum; ++i) {
            rad[i] = 1.0f;
        }
    }
    memset(mat->GetBuffer(), 0, sizeof(float) * mat->GetNum());
    
    if (indexnum) {
        std::map<std::pair<unsigned int, unsigned int>, int>::iterator it, eit = lines.end();
        unsigned int cnt = 0;
        unsigned int* p = idx->GetBuffer();
        for (it = lines.begin(); it != eit; ++it) {
            p[cnt] = it->first.first;  ++cnt;
            p[cnt] = it->first.second; ++cnt;
        }
    }
    
    return line;
}


bool OBJLoader::Load(const char* filename){
	Clear();
    
	m_obj = new SimpleObj();
	bool r = m_obj->Load(filename);
    if (!r) {
        delete m_obj;
        m_obj = 0;
        return false;
    }
    
	return r;
}

BufferMeshData *OBJLoader::MeshData()
{
    if (!m_mesh)
        m_mesh  = createMeshData(m_obj);
	return m_mesh;
}

BufferPointData *OBJLoader::PointData()
{
    if (!m_point)
        m_point = createPointData(m_obj);

    return m_point;
}

BufferLineData *OBJLoader::LineData()
{
    if (!m_line)
        m_line = createLineData(m_obj);
    
    return m_line;
}


