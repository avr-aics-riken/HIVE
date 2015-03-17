#include <string>

#include "TetraBuffer.h"
#include "BufferTetraData.h"
#include "../RenderObject/TetraModel.h"
#include "Buffer.h"
#include "Commands.h"

TetraBuffer::TetraBuffer(RENDER_MODE mode) : BaseBuffer(mode)
{
    m_model       = 0;
    m_vtxnum      = 0;
    m_vtx_id      = 0;
    m_material_id = 0;
}

TetraBuffer::~TetraBuffer()
{
    Clear();
}

void TetraBuffer::Clear()
{
    if (m_vtx_id)      ReleaseBufferVBIB_SGL(m_vtx_id);
    if (m_material_id) ReleaseBufferVBIB_SGL(m_material_id);
    m_vtx_id      = 0;
    m_material_id = 0;
    m_vtxnum      = 0;
    m_model       = 0;
}

bool TetraBuffer::Create(const TetraModel* model)
{
    bool r = true;
    if (!model) {
        fprintf(stderr,"Failed to create line: ");
        return false;
    }
    
    m_model = model;

    assert(0); // TODO: implement
    
    cacheTextures(model);
    
    return r;
}

void TetraBuffer::Render() const
{
    if (!m_model) {
        fprintf(stderr,"[Error] Not set vectormodel\n");
    }
    
    if (m_vtxnum == 0) {
        fprintf(stderr,"[Error] Invalide vector data\n");
        return;
    }

    bindUniforms(m_model);

    assert(0); // TODO: implement
    /*
    float w = 1.0;//m_model->GetLineWidth();
    LineWidth_SGL(w);
    
    BindLineVBIB_SGL(getProgram(), m_vtx_id, m_radius_id, m_material_id, m_idx_id);
    if (!m_indexnum)
        DrawLineArrays_SGL(m_vtxnum);
    else
        DrawLineElements_SGL(m_indexnum);
     */
}


