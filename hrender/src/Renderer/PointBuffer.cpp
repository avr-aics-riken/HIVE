#include <string>

#include "Analyzer.h"
#include "PointBuffer.h"
#include "BufferPointData.h"
#include "../RenderObject/PointModel.h"

#include "Commands.h"

PointBuffer::PointBuffer(RENDER_MODE mode) : BaseBuffer(mode)
{
    m_vtxnum      = 0;
    m_indexnum    = 0;
    m_vtx_id      = 0;
    m_radius_id   = 0;
    m_material_id = 0;
}

PointBuffer::~PointBuffer()
{
}

bool PointBuffer::Create(const PointModel* model)
{
    bool r = true;
    if (!model) {
        printf("Failed to create volume: ");
        return false;
    }

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
    m_indexnum = 0;

    //CreateVBRM_GL(particlenum, posbuffer, radiusbuffer, matbuffer,
    //        m_ogl.vtx, m_ogl.radius, m_ogl.material);
    //m_ogl.vtxnum   = particlenum;
    //m_ogl.indexnum = 0;

    return r;
}

void PointBuffer::Render() const
{
    //VX::Math::matrix mat = getTransformMatrix();↲
    //SetUniformMatrix_GL(m_sgl.prog, "lWorld", &mat.f[0]);↲
    //bindShaderParams(mode, m_sgl.prog);
    BindPointVB_SGL(getProgram(), m_vtx_id, m_radius_id, m_material_id);
    DrawPointArrays_SGL(m_vtxnum);
}


