#include <string>

#include "PolygonBuffer.h"
#include "../RenderObject/PolygonModel.h"
#include "../Buffer/BufferMeshData.h"

#include "Commands.h"

PolygonBuffer::PolygonBuffer(RENDER_MODE mode) : BaseBuffer(mode)
{
    m_vtx_id     = 0;
    m_normal_id  = 0;
    m_mat_id     = 0;
    m_tex_id     = 0;
    m_index_id   = 0;
    m_vertex_num = 0;
    m_index_num  = 0;
}
PolygonBuffer::~PolygonBuffer()
{
}

bool PolygonBuffer::Create(const PolygonModel* model)
{
    bool r = true;
    
    // load shader
    const std::string& shadername = model->GetShader();

    r &= loadShaderSrc(shadername.c_str());
    if (!r) {
        printf("[Error]Not set shader\n");
    }
        
    // make VB/IB
    BufferMeshData* mesh = model->GetMesh();
    const int vnum = mesh->Position()->GetNum();
    const int inum = mesh->Index()->GetNum();
    
    m_vertex_num = vnum;
    m_index_num  = inum;
    CreateVBIB_SGL(vnum, mesh->Position()->GetBuffer(),
                   mesh->Normal()->GetBuffer(),
                   mesh->Material()->GetBuffer(),
                   mesh->Texcoord()->GetBuffer(),
                   inum, mesh->Index()->GetBuffer(),
                   m_vtx_id, m_normal_id, m_mat_id, m_tex_id, m_index_id);
    
    
    return r;
}
void PolygonBuffer::Render() const
{
    BindVBIB_SGL(getProgram(), m_vtx_id, m_normal_id, m_mat_id, m_tex_id, m_index_id);
    if (m_index_id)
        DrawElements_SGL(m_index_num);
    else
        DrawArrays_SGL(m_vertex_num);
}


