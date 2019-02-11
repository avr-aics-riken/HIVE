/**
 * @file PointBuffer.cpp
 * ポイントバッファ
 */
#include <string>

#include "PointBuffer.h"
#include "BufferPointData.h"
#include "../RenderObject/PointModel.h"
#include "Buffer.h"

/// コンストラクタ
PointBuffer::PointBuffer(RenderDevice* render) : BaseBuffer(render)
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
    if (m_vtx_id)      ReleaseBuffer(m_vtx_id);
    if (m_radius_id)   ReleaseBuffer(m_radius_id);
    if (m_material_id) ReleaseBuffer(m_material_id);
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
    CreateVBRM(
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
    
    BindPointVB(getProgram(), m_vtx_id, m_radius_id, m_material_id);
    DrawPointArrays(m_vtxnum);
    UnBindPointVB(getProgram());
    unbindExtraBuffers(m_model);
}

void PointBuffer::Update()
{
    cacheTextures(m_model);
}

