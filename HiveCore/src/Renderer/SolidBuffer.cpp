/**
 * @file SolidBuffer.cpp
 * Solidバッファ
 */
#include <string>

#include "SolidBuffer.h"
#include "BufferSolidData.h"
#include "../RenderObject/SolidModel.h"
#include "Buffer.h"
#include "Commands.h"

/// コンストラクタ
SolidBuffer::SolidBuffer(RenderPlugin* render) : BaseBuffer(render)
{
    m_model       = 0;
    m_vtxnum      = 0;
    m_vtx_id      = 0;
    m_material_id = 0;
}

/// デストラクタ
SolidBuffer::~SolidBuffer()
{
    Clear();
}

/// クリア
void SolidBuffer::Clear()
{
    if (m_vtx_id)      ReleaseBufferVBIB_SGL(m_vtx_id);
    if (m_material_id) ReleaseBufferVBIB_SGL(m_material_id);
    m_vtx_id      = 0;
    m_material_id = 0;
    m_vtxnum      = 0;
    m_model       = 0;
}

/**
 * Solidバッファの作成.
 * @param model Solidモデル.
 */
bool SolidBuffer::Create(const SolidModel* model)
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
        fprintf(stderr,"[Error] A shader is not set\n");
        return false;
    }

    BufferSolidData* solid = model->GetSolid();
    if (!solid) {
        fprintf(stderr,"[Error] Invalid solid data\n");
        return false;
    }
    const int vnum = solid->Position()->GetNum();
    
    m_vtxnum = vnum;
    if (vnum == 0)
        return false;
    
	// Create solid VB/IB
	unsigned int normal_id, mat_id, tex_id, index_id;
    CreateVBIB_SGL(vnum, solid->Position()->GetBuffer(),
                   /* normal */ 0,
                   /* material(todo) */ 0,
                   /* texcoord */ 0,
                   /* numIndices */ 0,
				   /* indices */ 0,
                   m_vtx_id, normal_id, mat_id, tex_id, index_id);
    
    createExtraBuffers(m_model);
    
    cacheTextures(model);
    
    return r;
}

/**
 * レンダー.
 */
void SolidBuffer::Render() const
{
    if (!m_model) {
        fprintf(stderr,"[Error] Not set vectormodel\n");
    }
    
    if (m_vtxnum == 0) {
        fprintf(stderr,"[Error] Invalid solid data\n");
        return;
    }

    bindUniforms(m_model);

    bindExtraBuffers(m_model);

    BindSolidVBIB_SGL(getProgram(), m_vtx_id, 0, 0);
	// Use draw array method.
    DrawSolidArrays_SGL(m_model->GetSolid()->Type(), m_vtxnum);
    
    UnBindSolidVBIB_SGL(getProgram());
    unbindExtraBuffers(m_model);
}

void SolidBuffer::Update()
{
    cacheTextures(m_model);
}


