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
    m_model      = 0;
}
PolygonBuffer::~PolygonBuffer()
{
}

bool PolygonBuffer::Create(const PolygonModel* model)
{
    bool r = true;
    
    m_model = model;
    
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
    if (!m_model) {
        printf("[Error] Not setpolygonmodel\n");
    }
    
    const VX::Math::vec3& scale = m_model->GetScale();
    
    const unsigned int prg = getProgram();
    const VX::Math::vec3& translate = m_model->GetTranslate();
    SetUniform3fv_SGL(prg, "offset", reinterpret_cast<const float*>(&translate));
    SetUniform3fv_SGL(prg, "scale", reinterpret_cast<const float*>(&scale));
    
    const std::map<std::string, VX::Math::vec4>& vec4array = m_model->GetVec4();
    std::map<std::string, VX::Math::vec4>::const_iterator it4, eit4 = vec4array.end();
    for (it4 = vec4array.begin(); it4 != eit4; ++it4) {
        //SetUniform4f_SGL(prg, it->first.c_str(), it->second.x, it->second.y, it->second.z, it->second.w);
        VX::Math::vec4 v4 = it4->second;
        SetUniform4fv_SGL(prg, it4->first.c_str(), (const float*)&v4);
    }
    const std::map<std::string, VX::Math::vec3>& vec3array = m_model->GetVec3();
    std::map<std::string, VX::Math::vec3>::const_iterator it3, eit3 = vec3array.end();
    for (it3 = vec3array.begin(); it3 != eit3; ++it3) {
        const VX::Math::vec4& v3 = it3->second;
        SetUniform3fv_SGL(prg, it3->first.c_str(), (const float*)&v3);
    }
    const std::map<std::string, VX::Math::vec2>& vec2array = m_model->GetVec2();
    std::map<std::string, VX::Math::vec2>::const_iterator it2, eit2 = vec2array.end();
    for (it2 = vec2array.begin(); it2 != eit2; ++it2) {
        const VX::Math::vec4& v2 = it2->second;
        SetUniform2fv_SGL(prg, it2->first.c_str(), (const float*)&v2);
    }
    /*std::map<std::string, float>& floatarray = m_model->GetFloat();
     std::map<std::string, float>::iterator itf, eitf = floatarray.end();
     for (itf = floatarray.begin(); itf != eitf; ++itf) {
     const float vf = itf->second;
     SetUniform1f_SGL(prg, it2->first.c_str(), vf);
     }*/

    
    BindVBIB_SGL(getProgram(), m_vtx_id, m_normal_id, m_mat_id, m_tex_id, m_index_id);
    if (m_index_id)
        DrawElements_SGL(m_index_num);
    else
        DrawArrays_SGL(m_vertex_num);
}


