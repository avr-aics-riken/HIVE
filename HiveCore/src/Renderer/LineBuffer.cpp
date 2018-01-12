/**
 * @file LineBuffer.cpp
 * ラインバッファ
 */
#include <string>

#include "LineBuffer.h"
#include "BufferLineData.h"
#include "../RenderObject/LineModel.h"
#include "Buffer.h"
#include "Commands.h"


namespace {
    void (* const ReleaseBufferVBIB_GS[])(unsigned int buffer_id) = {ReleaseBufferVBIB_GL, ReleaseBufferVBIB_SGL};
    void (* const BindLineVBIB_GS[])(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material, unsigned int indexidx) = {BindLineVBIB_GL, BindLineVBIB_SGL};
    void (* const UnBindLineVBIB_GS[])(unsigned int prg) = {UnBindLineVBIB_GL, UnBindLineVBIB_SGL};
    void (* const DrawLineElements_GS[])(unsigned int indexnum) = {DrawLineElements_GL, DrawLineElements_SGL};
    void (* const DrawLineArrays_GS[])(unsigned int vtxnum) = {DrawLineArrays_GL, DrawLineArrays_SGL};
    void (* const CreateVBRM_GS[])(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                                   unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id) = {CreateVBRM_GL, CreateVBRM_SGL};
    void (* const CreateVBIBRM_GS[])(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                                     unsigned int indexnum, unsigned int* indexbuffer,
                                     unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id, unsigned int& index_id) = {CreateVBIBRM_GL, CreateVBIBRM_SGL};

    void (* const LineWidth_GS[])(float w) = {LineWidth_GL, LineWidth_SGL};
}


/// コンストラクタ
LineBuffer::LineBuffer(RenderPlugin* render) : BaseBuffer(render)
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
    if (m_vtx_id)      ReleaseBufferVBIB_GS[m_mode](m_vtx_id);
    if (m_radius_id)   ReleaseBufferVBIB_GS[m_mode](m_radius_id);
    if (m_material_id) ReleaseBufferVBIB_GS[m_mode](m_material_id);
    if (m_idx_id)      ReleaseBufferVBIB_GS[m_mode](m_idx_id);
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
        CreateVBRM_GS[m_mode](linePointNum,
                       line->Position()->GetBuffer(),
                       line->Radius()->GetBuffer(),
                       line->Material()->GetBuffer(),
                       m_vtx_id, m_radius_id, m_material_id);
    } else {
        CreateVBIBRM_GS[m_mode](linePointNum,
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
    LineWidth_GS[m_mode](w);
    
    BindLineVBIB_GS[m_mode](getProgram(), m_vtx_id, m_radius_id, m_material_id, m_idx_id);
    if (!m_indexnum)
        DrawLineArrays_GS[m_mode](m_vtxnum);
    else
        DrawLineElements_GS[m_mode](m_indexnum);
        
    UnBindLineVBIB_GS[m_mode](getProgram());
    unbindExtraBuffers(m_model);
}

void LineBuffer::Update()
{
    cacheTextures(m_model);
}

