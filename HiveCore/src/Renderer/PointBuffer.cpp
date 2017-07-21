/**
 * @file PointBuffer.cpp
 * ポイントバッファ
 */
#include <string>

#include "PointBuffer.h"
#include "BufferPointData.h"
#include "../RenderObject/PointModel.h"
#include "Buffer.h"
#include "Commands.h"

namespace {
    void (* const ReleaseBufferVBIB_GS[])(unsigned int buffer_id) = {ReleaseBufferVBIB_GL, ReleaseBufferVBIB_SGL};
    void (* const BindPointVB_GS[])(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material) = {BindPointVB_GL, BindPointVB_SGL};
    void (* const UnBindPointVB_GS[])(unsigned int prg) = {UnBindPointVB_GL, UnBindPointVB_SGL};
    void (* const DrawPointArrays_GS[])(unsigned int vtxnum) = {DrawPointArrays_GL, DrawPointArrays_SGL};
    void (* const CreateVBRM_GS[])(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                                   unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id) = {CreateVBRM_GL, CreateVBRM_SGL};

}


/// コンストラクタ
PointBuffer::PointBuffer(RENDER_MODE mode) : BaseBuffer(mode)
{
    m_vtxnum      = 0;
    m_model       = 0;
    m_vtx_id      = 0;
    m_radius_id   = 0;
    m_material_id = 0;
}

/// デストラクタ
PointBuffer::~PointBuffer()
{
}

/// クリア
void PointBuffer::Clear()
{
    if (m_vtx_id)      ReleaseBufferVBIB_GS[m_mode](m_vtx_id);
    if (m_radius_id)   ReleaseBufferVBIB_GS[m_mode](m_radius_id);
    if (m_material_id) ReleaseBufferVBIB_GS[m_mode](m_material_id);
    m_vtx_id      = 0;
    m_radius_id   = 0;
    m_material_id = 0;
    m_vtxnum      = 0;
}

/**
 * ポイントバッファの作成.
 * @param model ポイントモデル.
 */
bool PointBuffer::Create(const PointModel* model)
{
    bool r = true;
    if (!model) {
        fprintf(stderr,"Failed to create point: ");
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

    // make PointData
    BufferPointData *point = model->GetPoint();
    if (!point) {
        fprintf(stderr, "[Error]Invalid point data\n");
        return false;
    }
        
    unsigned int particlenum = point->Position()->GetNum();
    m_vtxnum   = particlenum;
    if (particlenum <= 0) {
        fprintf(stderr,"[Error]Point vertex empty\n");
        return false;
    }
    CreateVBRM_GS[m_mode](
            particlenum,
            point->Position()->GetBuffer(),
            point->Radius()->GetBuffer(),
            point->Material()->GetBuffer(),
            m_vtx_id, m_radius_id, m_material_id);
    
    createExtraBuffers(m_model);

    cacheTextures(m_model);
    
    return r;
}

/**
 * レンダー.
 */
void PointBuffer::Render() const
{
    if (!m_model) {
        fprintf(stderr,"[Error] Not setpointmodel\n");
    }
    
    if (m_vtxnum == 0) {
        fprintf(stderr,"[Error] Not invalid point data\n");
        return;
    }
    
    bindUniforms(m_model);
    
    bindExtraBuffers(m_model);
    
    BindPointVB_GS[m_mode](getProgram(), m_vtx_id, m_radius_id, m_material_id);
    DrawPointArrays_GS[m_mode](m_vtxnum);
    UnBindPointVB_GS[m_mode](getProgram());
    unbindExtraBuffers(m_model);
}

void PointBuffer::Update()
{
    cacheTextures(m_model);
}

