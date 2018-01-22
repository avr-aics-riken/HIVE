/**
 * @file PolygonBuffer.cpp
 * ポリゴンバッファ
 */
#include <string>

#include "PolygonBuffer.h"
#include "../RenderObject/PolygonModel.h"
#include "../Buffer/BufferMeshData.h"
#include "Buffer.h"

/// コンストラクタ.
PolygonBuffer::PolygonBuffer(RenderDevice* render) : BaseBuffer(render)
{
    m_vtx_id     = 0;
    m_normal_id  = 0;
    m_mat_id     = 0;
    m_tex_id     = 0;
    m_index_id   = 0;
    m_vertex_num = 0;
    m_index_num  = 0;
    m_model      = 0;
}

/// デストラクタ.
PolygonBuffer::~PolygonBuffer()
{
    Clear();
}

/// クリア
void PolygonBuffer::Clear()
{
    m_model = 0;
    if (m_vtx_id)    ReleaseBuffer(m_vtx_id);
    if (m_mat_id)    ReleaseBuffer(m_mat_id);
    if (m_normal_id) ReleaseBuffer(m_normal_id);
    if (m_tex_id)    ReleaseBuffer(m_tex_id);
    if (m_index_id)  ReleaseBuffer(m_index_id);
    m_vtx_id     = 0;
    m_normal_id  = 0;
    m_mat_id     = 0;
    m_tex_id     = 0;
    m_index_id   = 0;
    m_vertex_num = 0;
    m_index_num  = 0;
}

/**
 * ポリゴンバッファの作成
 * @param model ポリゴンモデル.
 */
bool PolygonBuffer::Create(const PolygonModel* model)
{
    if (!model) {
        fprintf(stderr,"[Error] Invalid polygon model\n");
        return false;
    }
    
    m_model = model;
    
    // load shader
    const std::string& shadername = model->GetShader();

    bool r = loadShaderSrc(shadername.c_str());
    if (!r) {
        fprintf(stderr,"[Error]Not set shader\n");
    }
        
    // make VB/IB
    BufferMeshData* mesh = model->GetMesh();
    if (!mesh) {
        fprintf(stderr,"[Error] Invalid mesh data\n");
        return false;
    }
    const int vnum = mesh->Position()->GetNum();
    const int inum = mesh->Index()->GetNum();
    
    m_vertex_num = vnum;
    m_index_num  = inum;
    if (vnum == 0)
        return false;
    
    CreateVBIB(vnum, mesh->Position()->GetBuffer(),
                   mesh->Normal()->GetBuffer(),
                   mesh->Material()->GetBuffer(),
                   mesh->Texcoord()->GetBuffer(),
                   inum, mesh->Index()->GetBuffer(),
                   m_vtx_id, m_normal_id, m_mat_id, m_tex_id, m_index_id);
    
    createExtraBuffers(m_model);
    
    cacheTextures(model);
    return r;
}

/**
 * レンダー.
 */
void PolygonBuffer::Render() const
{
    if (!m_model) {
        fprintf(stderr,"[Error] Not set polygonmodel\n");
    }
    
    if (m_vertex_num == 0) {
        fprintf(stderr,"[Error] Invalid vertex data\n");
        return;
    }
    
    bindUniforms(m_model);
    
    bindExtraBuffers(m_model);
    
    BindVBIB(getProgram(), m_vtx_id, m_normal_id, m_mat_id, m_tex_id, m_index_id);
    if (m_index_id)
        DrawElements(m_index_num);
    else
        DrawArrays(m_vertex_num);

    UnBindVBIB(getProgram());
    unbindExtraBuffers(m_model);
}

void PolygonBuffer::Update()
{
    cacheTextures(m_model);
}


