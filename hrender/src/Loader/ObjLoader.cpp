/**
 * @file ObjLoader.cpp
 * OBJデータローダー
 */

#include "ObjLoader.h"
#include "SimpleObj.h"
#include "BufferMeshData.h"
#include "BufferPointData.h"
#include "BufferLineData.h"
#include "BufferTetraData.h"
#include "BufferVectorData.h"
#include "BufferExtraData.h"
#include "Buffer.h"

/// コンストラクタ
OBJLoader::OBJLoader()
{
    m_obj    = 0;
    Clear();
}

/// デストラクタ
OBJLoader::~OBJLoader()
{
    Clear();
}

/// メンバクリア
void OBJLoader::Clear()
{
    delete m_obj;
    m_obj    = 0;
    m_mesh   = 0;
    m_point  = 0;
    m_line   = 0;
    m_normal = 0;
    m_tetra  = 0;
}

/**
 * メッシュデータの生成
 * @param  obj SimpleObjの参照
 * @retval BufferMeshData* メッシュデータ
 */
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

/**
 * 点データの生成
 * @param  obj SimpleObjの参照
 * @retval BufferPointData* Pointデータバッファ
 */
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

/**
 * 線データの生成
 * @param  obj SimpleObjの参照
 * @retval BufferLineData* Lineデータバッファ
 */
BufferLineData* OBJLoader::createLineData(const SimpleObj* obj) const
{
    BufferLineData* line = new BufferLineData();
    if (!obj) {
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

/**
 * Tetraデータの生成
 * @param  obj SimpleObjの参照
 * @retval BufferTetraData* Tetraデータバッファ
 */
BufferTetraData* OBJLoader::createTetraData(const SimpleObj* obj) const
{
    BufferTetraData* tetra = new BufferTetraData();
    if (!obj) {
        return tetra;
    }
    
    assert(0); // TODO: implementation
    
    return tetra;
}

/**
 * Vectorデータの生成
 * @param  obj SimpleObjの参照
 * @retval BufferVectorData* Vectorデータバッファ
 */
BufferVectorData* OBJLoader::createNormalVectorData(const SimpleObj* obj) const
{
    BufferVectorData* vec = new BufferVectorData();
    if (!obj) {
        return vec;
    }
    
    const int vnum = obj->GetVertexNum();
    vec->Create(vnum);
    Vec3Buffer* pos    = vec->Position();
    Vec3Buffer* normal = vec->Direction();
    memcpy(pos->GetBuffer(),    obj->GetPositionBuffer(), sizeof(float) * 3 * vnum);
    memcpy(normal->GetBuffer(), obj->GetNormalBuffer(),   sizeof(float) * 3 * vnum);
    
    return vec;
}

/**
 * Objデータロード
 * @param  filename ファイルパス
 * @retval true  成功
 * @retval false 失敗
 */
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

/**
 * メッシュデータバッファ取得
 * @retval BufferMeshData* メッシュデータバッファ
 */
BufferMeshData *OBJLoader::MeshData()
{
    if (!m_mesh)
        m_mesh  = createMeshData(m_obj);
	return m_mesh;
}

/**
 * 点データバッファ取得
 * @retval BufferPointData* 点データバッファ
 */
BufferPointData *OBJLoader::PointData()
{
    if (!m_point)
        m_point = createPointData(m_obj);

    return m_point;
}

/**
 * 線データバッファ取得
 * @retval BufferLineData* 線データバッファ
 */
BufferLineData *OBJLoader::LineData()
{
    if (!m_line)
        m_line = createLineData(m_obj);
    
    return m_line;
}

/**
 * Tetraデータバッファ取得
 * @retval BufferTetraData* Tetraデータバッファ
 */
BufferTetraData *OBJLoader::TetraData()
{
    if (!m_tetra)
        m_tetra = createTetraData(m_obj);
    
    return m_tetra;
}

/**
 * Vectorデータバッファ取得
 * @retval BufferVectorData* Vectorデータバッファ
 */
BufferVectorData *OBJLoader::NormalData()
{
    if (!m_normal)
        m_normal = createNormalVectorData(m_obj);
    
    return m_normal;
}

/**
 * Extraデータバッファ取得(テスト用)
 * @retval BufferExtraData* Extraデータバッファ
 */
BufferExtraData *OBJLoader::ExtraData()
{
    if (!m_extra)
        m_extra = createExtraData(m_obj);
    
    return m_extra;
}

// FOR TEST
BufferExtraData* OBJLoader::createExtraData(const SimpleObj* obj) const
{
    BufferExtraData* vec = new BufferExtraData();
    if (!obj) {
        return vec;
    }
    
    const int vnum = obj->GetVertexNum();
    vec->Create("vec3", vnum);
    Vec3Buffer* extra = vec->Vec3Buffer();
    memcpy(extra->GetBuffer(), obj->GetNormalBuffer(), sizeof(float) * 3 * vnum);
    for (int i = 0; i < vnum; ++i) {
        extra->GetBuffer()[3*i    ] = 1.0 - extra->GetBuffer()[3*i    ];
        extra->GetBuffer()[3*i + 1] = 1.0 - extra->GetBuffer()[3*i + 1];
        extra->GetBuffer()[3*i + 2] = 1.0 - extra->GetBuffer()[3*i + 2];
    }
    
    return vec;
}


