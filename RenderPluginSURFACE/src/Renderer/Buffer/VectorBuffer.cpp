/**
 * @file VectorBuffer.cpp
 * ベクトルバッファ
 */
#include <string>

#include "VectorBuffer.h"
#include "BufferVectorData.h"
#include "../RenderObject/VectorModel.h"
#include "Buffer.h"
#include "../Core/vxmath.h"

/// コンストラクタ
VectorBuffer::VectorBuffer(RenderDevice* render) : BaseBuffer(render)
{
    m_line_vtx_id    = 0;
    m_tetra_vtx_id   = 0;
    Clear();
}

/// デストラクタ
VectorBuffer::~VectorBuffer()
{
    Clear();
}

/// クリア
void VectorBuffer::Clear()
{
    if (m_line_vtx_id)  ReleaseBuffer(m_line_vtx_id);
    if (m_tetra_vtx_id) ReleaseBuffer(m_tetra_vtx_id);
    if (m_mat_id)       ReleaseBuffer(m_mat_id);
    m_line_vtx_id    = 0;
    m_tetra_vtx_id   = 0;
    m_mat_id         = 0;
    m_lines_vnum     = 0;
    m_tetra_vnum     = 0;
}

/**
 * ベクトルバッファの作成.
 * @param model ベクトルモデル.
 */
bool VectorBuffer::Create(const VectorModel* model)
{
    bool r = true;
    if (!model) {
        fprintf(stderr,"Failed to create line: ");
        return false;
    }

    Clear();
    
    m_model = model;

    // load shader
    const std::string& shadername = model->GetShader();
    printf("%s\n", shadername.c_str());
    r &= loadShaderSrc(shadername.c_str());
    if (!r) {
        fprintf(stderr,"[Error]Not set shader\n");
    }

    // make VectorData
    const BufferVectorData* vec = model->GetVector();
    if (!vec) {
        fprintf(stderr,"[Error]Invalid vector data\n");
        return false;
    }
    
    const int vecNum = vec->Position()->GetNum();
    if (vecNum <= 0) {
        fprintf(stderr,"[Error]Vector vertex empty\n");
        return false;
    }
    
    m_lines_vnum = vecNum * 2;
    m_tetra_vnum = vecNum * 4;

    // Create Line data
    RefPtr<Vec3Buffer> lineBuf = new Vec3Buffer();
    lineBuf->Create(m_lines_vnum);
    float* l3 = lineBuf->GetBuffer();
    const float* v3 = vec->Position()->GetBuffer();
    const float* d3 = vec->Direction()->GetBuffer();
    const float lengthScale = m_model->GetLengthScale();
    const float arrowSize   = m_model->GetArrowSize();
    const float lineWidth   = m_model->GetLineWidth();
    printf("LengthScale = %.5f, arrowSize = %.5f, LineWidth = %.5f\n", lengthScale, arrowSize, lineWidth);
    for (unsigned int i = 0; i < vecNum; ++i) {
        l3[2 * 3 * i    ] = v3[3 * i    ];
        l3[2 * 3 * i + 1] = v3[3 * i + 1];
        l3[2 * 3 * i + 2] = v3[3 * i + 2];
        l3[2 * 3 * i + 3] = v3[3 * i    ] + d3[3 * i    ] * lengthScale;
        l3[2 * 3 * i + 4] = v3[3 * i + 1] + d3[3 * i + 1] * lengthScale;
        l3[2 * 3 * i + 5] = v3[3 * i + 2] + d3[3 * i + 2] * lengthScale;
    }
    
    // Create Arrow data
    RefPtr<Vec3Buffer> tetraBuf = new Vec3Buffer();
    tetraBuf->Create(m_tetra_vnum);
    using namespace VX::Math;
    vec3* tv3 = reinterpret_cast<vec3*>(tetraBuf->GetBuffer());
    for (unsigned int i = 0; i < vecNum; ++i) {
        vec3 dir = vec3(d3[3 * i], d3[3 * i + 1], d3[3 * i + 2]);
        const vec3 center = vec3(v3[3 * i], v3[3 * i + 1], v3[3 * i + 2]) + dir * lengthScale;
        float len = length(dir);
        if (len < 0.00001) {
            tv3[4 * i    ] = vec3(0,0,0);
            tv3[4 * i + 1] = vec3(0,0,0);
            tv3[4 * i + 2] = vec3(0,0,0);
            tv3[4 * i + 3] = vec3(0,0,0);
        } else {
            dir = normalize(dir);
            const vec3 tangent = cross(dir, VX::Math::vec3(0,1,0)); // This is easy way, TODO: to calc correct tangent.
            tv3[4 * i    ] = center + dir * arrowSize;
            tv3[4 * i + 1] = center + tangent * arrowSize;
            tv3[4 * i + 2] = center + (RotationAxis(dir, 120) * vec4(tangent, 0.0)).xyz() * arrowSize;
            tv3[4 * i + 3] = center + (RotationAxis(dir,-120) * vec4(tangent, 0.0)).xyz() * arrowSize;
        }
    }

    // Create Line and tetra VB
    unsigned int normal_id, mat_id, tex_id, index_id;
    CreateVBIB(m_lines_vnum, lineBuf->GetBuffer(),  0, 0, 0, 0, 0, m_line_vtx_id,  normal_id, mat_id, tex_id, index_id);
    CreateVBIB(m_tetra_vnum, tetraBuf->GetBuffer(), 0, 0, 0, 0, 0, m_tetra_vtx_id, normal_id, mat_id, tex_id, index_id);

    createExtraBuffers(m_model);

    cacheTextures(model);
    
    return r;
}

/**
 * レンダー.
 */
void VectorBuffer::Render() const
{
    if (!m_model) {
        fprintf(stderr,"[Error] Not set vectormodel\n");
    }
    
    if (m_lines_vnum == 0 || m_tetra_vnum == 0) {
        fprintf(stderr,"[Error] Invalid vector data\n");
        return;
    }

    bindUniforms(m_model);
    
    bindExtraBuffers(m_model);
    
    const float w = m_model->GetLineWidth();
    LineWidth(w);
    
    BindLineVBIB(getProgram(), m_line_vtx_id, 0, 0, 0);
    DrawLineArrays(m_lines_vnum);
    UnBindLineVBIB(getProgram());
    
    BindTetraVBIB(getProgram(), m_tetra_vtx_id, 0, 0);
    DrawTetraArrays(m_tetra_vnum);
    UnBindTetraVBIB(getProgram());
    unbindExtraBuffers(m_model);
}

void VectorBuffer::Update()
{
    cacheTextures(m_model);
}


