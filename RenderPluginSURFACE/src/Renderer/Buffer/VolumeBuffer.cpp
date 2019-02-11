/**
 * @file VolumeBuffer.cpp
 * Volumeバッファ
 */
#include <string>

//#include "Analyzer.h"
#include "VolumeBuffer.h"
#include "BufferVolumeData.h"
#include "../RenderObject/VolumeModel.h"
#include "../Buffer/BufferMeshData.h"
#include "Buffer.h"
#include <string.h>

/// コンストラクタ
VolumeBuffer::VolumeBuffer(RenderDevice* render) : BaseBuffer(render)
{
    m_vtx_id     = 0;
    m_normal_id  = 0;
    m_mat_id     = 0;
    m_tex_id     = 0;
    m_index_id   = 0;
    m_vertex_num = 0;
    m_index_num  = 0;
    memset(m_boxsize, 0, sizeof(m_boxsize));
    memset(m_voldim, 0, sizeof(m_voldim));
    m_volcomp = 0;
/*
    memset(m_volMinVal, 0, sizeof(m_volMinVal));
    memset(m_volMaxVal, 0, sizeof(m_volMaxVal));
    m_volHist[0].clear();
    m_volHist[1].clear();
    m_volHist[2].clear();
*/
    m_model      = 0;
}

/// デストラクタ
VolumeBuffer::~VolumeBuffer()
{
    Clear();
}

// バッファのクリア
void VolumeBuffer::Clear()
{
    if (m_vtx_id) ReleaseBuffer(m_vtx_id);
    if (m_normal_id) ReleaseBuffer(m_normal_id);
    if (m_mat_id) ReleaseBuffer(m_mat_id);
    if (m_tex_id) ReleaseBuffer(m_tex_id);
    if (m_index_id) ReleaseBuffer(m_index_id);
}

/**
 * ボリュームレンダリング用ボックスモデル作成
 * @param width  width 
 * @param height height
 * @param depth  depth 
 * @retval true 作成成功
 * @retval false 作成失敗
 */
bool VolumeBuffer::MakeBox(float width, float height, float depth)
{
    float vtx[] = {
        -width*.5f, height*.5f,-depth*.5f,
        width*.5f, height*.5f,-depth*.5f,
        -width*.5f,-height*.5f,-depth*.5f,
        width*.5f,-height*.5f,-depth*.5f,
        width*.5f, height*.5f, depth*.5f,
        -width*.5f, height*.5f, depth*.5f,
        width*.5f,-height*.5f, depth*.5f,
        -width*.5f,-height*.5f, depth*.5f
    };
    float* normal = vtx;
    unsigned int index[] = { 0,1,2, 1,3,2,  1,4,3, 4,6,3,  4,5,6, 5,7,6,  5,0,7, 0,2,7,  5,4,0, 4,1,0,  2,3,7, 3,6,7 };
    const unsigned int vertexnum = sizeof(vtx)/sizeof(float)/3;
    const unsigned int indexnum  = sizeof(index)/sizeof(unsigned int);
    float mat[vertexnum*3] = {0};

    CreateVBIB(vertexnum, &vtx[0], &normal[0], &mat[0], 0, indexnum, &index[0],
            m_vtx_id, m_normal_id, m_mat_id, m_tex_id, m_index_id);
    m_vertex_num = vertexnum;
    m_index_num  = indexnum;

    //CreateVBIB_GL(vertexnum, &vtx[0], &normal[0], &mat[0], 0, indexnum, &index[0],
    //        m_ogl.vtx, m_ogl.normal, m_ogl.material, m_sgl.tex, m_ogl.index);
    //m_ogl.vtxnum   = vertexnum;
    //m_ogl.indexnum = indexnum;

    m_boxsize[0] = width;
    m_boxsize[1] = height;
    m_boxsize[2] = depth;

    return true;
}

/**
 * 3Dテクスチャ作成
 * @param width  width 
 * @param height height
 * @param depth  depth 
 * @param component  component 
 * @param volumedata  volumedata 
 * @retval true 作成成功
 * @retval false 作成失敗
 */
bool VolumeBuffer::CreateTexture3D(unsigned int width, unsigned int height, unsigned int depth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR)
{
    printf("CreateTexture3D(%d,%d,%d,%d)\n", width, height, depth, component);
    GenTextures(1, &m_sgl_voltex);
    BindTexture3D(m_sgl_voltex);
    TexImage3DPointer(width, height, depth, component, volumedata, clampToEdgeS, clampToEdgeT, clampToEdgeR);
    m_voldim[0] = width;
    m_voldim[1] = height;
    m_voldim[2] = depth;
    m_volcomp   = component;

/*
    VolumeAnalyzerProc analyze;
    int dim[] = {static_cast<int>(width), static_cast<int>(height), static_cast<int>(depth)};
    if (component == 1) {
        analyze.AnalyzeScalar(m_volHist[0], m_volMinVal[0], m_volMaxVal[0], (const float*)volumedata, dim);
        for (int i = 1; i < 3; ++i) {
            m_volHist[i] = m_volHist[0];
            m_volMinVal[i] = m_volMinVal[0];
            m_volMaxVal[i] = m_volMaxVal[0];
        }
    } else if (component == 3) {
        analyze.AnalyzeVector(m_volHist, m_volMinVal, m_volMaxVal, (const float*)volumedata, dim);
    } else {
        analyze.AnalyzeVector(m_volHist, m_volMinVal, m_volMaxVal, (const float*)volumedata, dim, 256, component);
    }
*/
    return true;
}

/**
 * Volumeバッファ作成
 * @param model  Volumeモデル 
 * @retval true  作成成功
 * @retval false 作成失敗
 */
bool VolumeBuffer::Create(const VolumeModel* model)
{
    bool r = true;
    if (!model) {
        fprintf(stderr,"Failed to create volume\n");
        return false;
    }

    //todo constant behavior.
    //m_model = reinterpret_cast<VolumeModel*>(model);
    m_model = model;

    // load shader
    const std::string& shadername = model->GetShader();
    printf("%s\n", shadername.c_str());
    r &= loadShaderSrc(shadername.c_str());
    if (!r) {
        fprintf(stderr,"[Error]Not set shader\n");
    }

    // make VolumeData
    BufferVolumeData* volume = model->GetVolume();
    if(!volume) {
        fprintf(stderr,"[Error] BufferVolumeData is NULL\n");
        return false;
    }
    if(volume->Buffer()) {
        float sw = volume->Width();
        float sh = volume->Height();
        float sd = volume->Depth();
        r = CreateTexture3D(sw, sh, sd, volume->Component(), volume->Buffer()->GetBuffer(), model->GetClampToEdgeS(), model->GetClampToEdgeT(), model->GetClampToEdgeR());
		if (volume->NonUniform()) {
			// Register remap table for non-uniform volume
    		BindTexture3D(m_sgl_voltex);
			TexCoordRemap3D(0 /* = x */, volume->SpacingX()->GetNum(), volume->SpacingX()->GetBuffer());
			TexCoordRemap3D(1 /* = y */, volume->SpacingY()->GetNum(), volume->SpacingY()->GetBuffer());
			TexCoordRemap3D(2 /* = z */, volume->SpacingZ()->GetNum(), volume->SpacingZ()->GetBuffer());
		}

        MakeBox(sw,sh,sd);
    } else {
        fprintf(stderr,"[Error] Not load buffer\n");
    }
    
    createExtraBuffers(m_model);

    cacheTextures(model);
    
    return r;
}

/**
 * レンダー.
 */
void VolumeBuffer::Render() const
{
    if (!m_model) {
        fprintf(stderr,"[Error] Not set volume\n");
    }

    // TODO: not supported yet rotation
    //VX::Math::vec3 scale = m_model->GetScale();
    //float volumescale[] = {
    //    m_boxsize[0] * scale.x,
    //    m_boxsize[1] * scale.y,
    //    m_boxsize[2] * scale.z
    //};
    float volumescale[] = {m_boxsize[0],m_boxsize[1],m_boxsize[2]};
    printf("VolumeBuffer Size = (%f %f %f)\n", volumescale[0], volumescale[1], volumescale[2]);
    
    const unsigned int prg = getProgram();
    SetUniform3fv(prg, "volumescale", volumescale);
    SetUniform3fv(prg, "volumedim", m_voldim);
    SetUniform1i(prg, "volumecomponent", m_volcomp);

    VX::Math::vec3 translate = m_model->GetTranslate();
    SetUniform3fv(prg, "offset", (float *)&translate);
    
    bindUniforms(m_model);
    
    bindExtraBuffers(m_model);
    
    BindVBIB(getProgram(), m_vtx_id, m_normal_id, m_mat_id, m_tex_id, m_index_id);
    BindTexture3D(m_sgl_voltex);
    SetUniform1i(getProgram(), "tex0", 0);
    if (m_index_id)
    {
        DrawElements(m_index_num);
    } else {
        DrawArrays(m_vertex_num);
    }
    UnBindVBIB(getProgram());
    unbindExtraBuffers(m_model);
}

void VolumeBuffer::Update()
{
    cacheTextures(m_model);
}



