/**
 * @file SparseVolumeBuffer.cpp
 * Volumeバッファ
 */
#include <string>
#include <string.h>

//#include "Analyzer.h"
#include "SparseVolumeBuffer.h"
#include "BufferSparseVolumeData.h"
#include "../RenderObject/SparseVolumeModel.h"
#include "../Buffer/BufferMeshData.h"
#include "../Buffer/BufferVolumeData.h"
#include "Buffer.h"

/// コンストラクタ
SparseVolumeBuffer::SparseVolumeBuffer(RenderDevice* render) : BaseBuffer(render)
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
    memset(m_volMinVal, 0, sizeof(m_volMinVal));
    memset(m_volMaxVal, 0, sizeof(m_volMaxVal));
    m_model      = 0;
}

/// デストラクタ
SparseVolumeBuffer::~SparseVolumeBuffer()
{
}

/**
 * ボリュームレンダリング用ボックスモデル作成
 * @param width  width 
 * @param height height
 * @param depth  depth 
 * @retval true 作成成功
 * @retval false 作成失敗
 */
bool SparseVolumeBuffer::MakeBox(float width, float height, float depth)
{
    static float vtx[] = {
        -width*.5f, height*.5f,-depth*.5f,
        width*.5f, height*.5f,-depth*.5f,
        -width*.5f,-height*.5f,-depth*.5f,
        width*.5f,-height*.5f,-depth*.5f,
        width*.5f, height*.5f, depth*.5f,
        -width*.5f, height*.5f, depth*.5f,
        width*.5f,-height*.5f, depth*.5f,
        -width*.5f,-height*.5f, depth*.5f
    };
    static float* normal = vtx;
    static unsigned int index[] = { 0,1,2, 1,3,2,  1,4,3, 4,6,3,  4,5,6, 5,7,6,  5,0,7, 0,2,7,  5,4,0, 4,1,0,  2,3,7, 3,6,7 };
    const unsigned int vertexnum = sizeof(vtx)/sizeof(float)/3;
    const unsigned int indexnum  = sizeof(index)/sizeof(unsigned int);
    static float mat[vertexnum*3] = {0};

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
 * Sparse 3Dテクスチャ作成
 * @param width  width 
 * @param height height
 * @param depth  depth 
 * @param component  component 
 * @param volumedata  volumedata 
 * @retval true 作成成功
 * @retval false 作成失敗
 */
bool SparseVolumeBuffer::CreateSparseTexture3D(const BufferSparseVolumeData* volume, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR)
{
    printf("CreateSparseTexture3D(%d,%d,%d)\n", volume->Width(), volume->Height(), volume->Depth());
    GenTextures(1, &m_sgl_voltex);
    BindTexture3D(m_sgl_voltex);

	for (size_t i = 0; i < volume->VolumeBlocks().size(); i++) {
		const BufferSparseVolumeData::VolumeBlock& block = volume->VolumeBlocks()[i];

        if (block.isRaw) {
            SparseTexImage3DPointer(block.level, block.offset[0], block.offset[1], block.offset[2], block.extent[0], block.extent[1], block.extent[2], block.size[0], block.size[1], block.size[2], block.components, reinterpret_cast<const float*>(block.rawData), clampToEdgeS, clampToEdgeT, clampToEdgeR);
        } else {
            SparseTexImage3DPointer(block.level, block.offset[0], block.offset[1], block.offset[2], block.extent[0], block.extent[1], block.extent[2], block.volume->Width(), block.volume->Height(), block.volume->Depth(), block.volume->Component(), block.volume->Buffer()->GetBuffer(), clampToEdgeS, clampToEdgeT, clampToEdgeR);
        }
	}
    m_voldim[0] = volume->Width();
    m_voldim[1] = volume->Height();
    m_voldim[2] = volume->Depth();

	// @todo {}
    //VolumeAnalyzerProc analyze;
    //int dim[] = {static_cast<int>(width), static_cast<int>(height), static_cast<int>(depth)};
    //if (component == 1) {
    //    analyze.AnalyzeScalar(m_volHist[0], m_volMinVal[0], m_volMaxVal[0], (const float*)volumedata, dim);
    //    for (int i = 1; i < 3; ++i) {
    //        m_volHist[i] = m_volHist[0];
    //        m_volMinVal[i] = m_volMinVal[0];
    //        m_volMaxVal[i] = m_volMaxVal[0];
    //    }
    //} else if (component == 3) {
    //    analyze.AnalyzeVector(m_volHist, m_volMinVal, m_volMaxVal, (const float*)volumedata, dim);
    //} else {
    //    analyze.AnalyzeVector(m_volHist, m_volMinVal, m_volMaxVal, (const float*)volumedata, dim, 256, component);
    //}

    return true;
}

/**
 * Volumeバッファ作成
 * @param model  Volumeモデル 
 * @retval true  作成成功
 * @retval false 作成失敗
 */
bool SparseVolumeBuffer::Create(const SparseVolumeModel* model)
{
    bool r = true;
    if (!model) {
        fprintf(stderr,"Failed to create volume\n");
        return false;
    }

    m_model = (SparseVolumeModel*)(model);

    // load shader
    const std::string& shadername = model->GetShader();
    printf("%s\n", shadername.c_str());
    r &= loadShaderSrc(shadername.c_str());
    if (!r) {
        fprintf(stderr,"[Error]Not set shader\n");
    }

    // make VolumeData
    BufferSparseVolumeData* volume = model->GetSparseVolume();
    if(!volume) {
        fprintf(stderr,"[Error] BufferSparseVolumeData is NULL\n");
        return false;
    }

    if(volume->VolumeBlocks().size() > 0) {
        float sw = volume->Width();
        float sh = volume->Height();
        float sd = volume->Depth();
        r = CreateSparseTexture3D(volume, model->GetClampToEdgeS(), model->GetClampToEdgeT(), model->GetClampToEdgeR());
        MakeBox(sw,sh,sd);
    } else {
        fprintf(stderr,"[Error] Not load buffer\n");
    }
    
    cacheTextures(model);
    
    return r;
}

/**
 * レンダー.
 */
void SparseVolumeBuffer::Render() const
{
    if (!m_model) {
        fprintf(stderr,"[Error] Not set volume\n");
    }

    // TODO: not supported yet rotation
    VX::Math::vec3 scale = m_model->GetScale();
    //float volumescale[] = {
    //    m_boxsize[0] * scale.x,
    //    m_boxsize[1] * scale.y,
    //    m_boxsize[2] * scale.z
    //};
    float volumescale[] = {m_boxsize[0],m_boxsize[1],m_boxsize[2]};
    printf("SparseVolumeBuffer Size = (%f %f %f)\n", volumescale[0], volumescale[1], volumescale[2]);
    
    const unsigned int prg = getProgram();
    SetUniform3fv(prg, "volumescale", volumescale);
    SetUniform3fv(prg, "volumedim", m_voldim);
    VX::Math::vec3 translate = m_model->GetTranslate();
    SetUniform3fv(prg, "offset", (float *)&translate);
    
    bindUniforms(m_model);
    
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
}

void SparseVolumeBuffer::Update()
{
    cacheTextures(m_model);
}



