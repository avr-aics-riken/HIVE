#include <string>

#include "Analyzer.h"
#include "PointBuffer.h"
#include "BufferPointData.h"
#include "../RenderObject/PointModel.h"

#include "Commands.h"

PointBuffer::PointBuffer(RENDER_MODE mode) : BaseBuffer(mode)
{
    m_vtxnum      = 0;
    m_model       = 0;
    m_vtx_id      = 0;
    m_radius_id   = 0;
    m_material_id = 0;
}

PointBuffer::~PointBuffer()
{
}

void PointBuffer::Clear()
{
    if (m_vtx_id)      ReleaseBufferVBIB_SGL(m_vtx_id);
    if (m_radius_id)   ReleaseBufferVBIB_SGL(m_radius_id);
    if (m_material_id) ReleaseBufferVBIB_SGL(m_material_id);
    m_vtx_id      = 0;
    m_radius_id   = 0;
    m_material_id = 0;
    m_vtxnum      = 0;
}

bool PointBuffer::Create(const PointModel* model)
{
    bool r = true;
    if (!model) {
        printf("Failed to create point: ");
        return false;
    }
    
    m_model = model;

    // load shader
    const std::string& shadername = model->GetShader();
    printf("%s\n", shadername.c_str());
    r &= loadShaderSrc(shadername.c_str());
    if (!r) {
        printf("[Error]Not set shader\n");
        return false;
    }

    // make PointData
    BufferPointData *point = model->GetPoint();
    if (!point) {
        printf("[Error]Invalid point data\n");
        return false;
    }
        
    unsigned int particlenum = point->Position()->GetNum();
    if (particlenum <= 0) {
        printf("[Error]Point vertex empty\n");
        return false;
    }
    CreateVBRM_SGL(
            particlenum,
            point->Position()->GetBuffer(),
            point->Radius()->GetBuffer(),
            point->Material()->GetBuffer(),
            m_vtx_id, m_radius_id, m_material_id);
    m_vtxnum   = particlenum;

    CreateVBRM_GL(particlenum,
                  point->Position()->GetBuffer(),
                  point->Radius()->GetBuffer(),
                  point->Material()->GetBuffer(),
                  m_vtx_id, m_radius_id, m_material_id);
    
    return r;
}

void PointBuffer::Render() const
{
    if (!m_model) {
        printf("[Error] Not setpointmodel\n");
    }
    
    bindUniforms(m_model);
    
    BindPointVB_SGL(getProgram(), m_vtx_id, m_radius_id, m_material_id);
    DrawPointArrays_SGL(m_vtxnum);
}


