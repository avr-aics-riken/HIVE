/**
 * @file LineBuffer.cpp
 * ラインバッファ
 */
#include <string>

#include "LineBuffer.h"
#include "BufferLineData.h"
#include "../RenderObject/LineModel.h"
#include "Buffer.h"


/// コンストラクタ
LineBuffer::LineBuffer(RenderDevice* render) : BaseBuffer(render)
{
    m_vtxnum      = 0;
    m_indexnum    = 0;
    m_model       = 0;
    m_vtx_id      = 0;
    m_radius_id   = 0;
    m_material_id = 0;
    m_idx_id      = 0;
}

/// デストラクタ
LineBuffer::~LineBuffer()
{
    Clear();
}

/// クリア
void LineBuffer::Clear()
{
    if (m_vtx_id)      ReleaseBuffer(m_vtx_id);
    if (m_radius_id)   ReleaseBuffer(m_radius_id);
    if (m_material_id) ReleaseBuffer(m_material_id);
    if (m_idx_id)      ReleaseBuffer(m_idx_id);
    m_vtx_id      = 0;
    m_radius_id   = 0;
    m_material_id = 0;
    m_idx_id      = 0;
    m_vtxnum      = 0;
    m_indexnum    = 0;
}

/**
 * ラインバッファの作成.
 * @param model ラインモデル.
 */
bool LineBuffer::Create(const LineModel* model)
{
    bool r = true;
    if (!model) {
        fprintf(stderr,"Failed to create line: ");
        return false;
    }
    
    m_model = model;

    // load shader
    const std::string& shadername = model->GetShader();
    printf("%s\n", shadername.c_str());
    r &= loadShaderSrc(shadername.c_str());
    if (!r) {
        fprintf(stderr,"[Error]Not set shader\n");
    }

    // make LineData
    BufferLineData* line = model->GetLine();
    if (!line) {
        fprintf(stderr,"[Error]Invalid line data\n");
        return false;
    }
    unsigned int linePointNum = line->Position()->GetNum();
    if (linePointNum <= 0) {
        fprintf(stderr,"[Error]Point vertex empty\n");
        return false;
    }
    
    m_vtxnum   = linePointNum;
    m_indexnum = line->Index()->GetNum();
    if (linePointNum <= 0) {
        fprintf(stderr,"[Error]Line vertex empty\n");
        return false;
    }
    if (!m_indexnum) {
        CreateVBRM(linePointNum,
                       line->Position()->GetBuffer(),
                       line->Radius()->GetBuffer(),
                       line->Material()->GetBuffer(),
                       m_vtx_id, m_radius_id, m_material_id);
    } else {
        CreateVBIBRM(linePointNum,
                         line->Position()->GetBuffer(),
                         line->Radius()->GetBuffer(),
                         line->Material()->GetBuffer(),
                         m_indexnum,
                         line->Index()->GetBuffer(),
                         m_vtx_id, m_radius_id, m_material_id, m_idx_id);
    }
    
    createExtraBuffers(m_model);
    
    cacheTextures(m_model);

    return r;
}

/**
 * レンダー.
 */
void LineBuffer::Render() const
{
    if (!m_model) {
        fprintf(stderr,"[Error] Not set linemodel\n");
    }
    
    if (m_vtxnum == 0) {
        fprintf(stderr,"[Error] Invalide line data\n");
        return;
    }

    bindUniforms(m_model);
    
    bindExtraBuffers(m_model);
    
    float w = m_model->GetLineWidth();
    LineWidth(w);
    
    BindLineVBIB(getProgram(), m_vtx_id, m_radius_id, m_material_id, m_idx_id);
    if (!m_indexnum)
        DrawLineArrays(m_vtxnum);
    else
        DrawLineElements(m_indexnum);
        
    UnBindLineVBIB(getProgram());
    unbindExtraBuffers(m_model);
}

void LineBuffer::Update()
{
    cacheTextures(m_model);
}

