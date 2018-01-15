/**
 * @file BaseBuffer.cpp
 * ベースバッファ
 */
#include <string>
#include "BaseBuffer.h"
#include "../RenderObject/Camera.h"
#include "../RenderObject/RenderObject.h"
#include "../Buffer/Buffer.h"
#include "../Buffer/BufferImageData.h"
#include "../Buffer/BufferExtraData.h"

//#include "../Renderer/RenderCore.h"
#include "../Renderer/RenderPlugin.h"

#include "Commands.h"

namespace {
    void (* const SetUniform4fv_GS[])(unsigned int prg, const char* name, const float* val) = {SetUniform4fv_GL, SetUniform4fv_SGL};
    void (* const SetUniform3fv_GS[])(unsigned int prg, const char* name, const float* val) = {SetUniform3fv_GL, SetUniform3fv_SGL};
    void (* const SetUniform2fv_GS[])(unsigned int prg, const char* name, const float* val) = {SetUniform2fv_GL, SetUniform2fv_SGL};
    void (* const SetUniform1f_GS[])(unsigned int prg, const char* name, float val) = {SetUniform1f_GL, SetUniform1f_SGL};
    void (* const SetUniform1i_GS[])(unsigned int prg, const char* name, int val) = {SetUniform1i_GL, SetUniform1i_SGL};
    void (* const BindProgram_GS[])(unsigned int prg) = {BindProgram_GL, BindProgram_SGL};
    void (* const SetUniformMatrix_GS[])(unsigned int prg, const char* name, const float* val) = {SetUniformMatrix_GL, SetUniformMatrix_SGL};
    
    void (* const CreateFloatBuffer_GS[])(unsigned int num, float* buffer, unsigned int& buf_id) = {CreateFloatBuffer_GL, CreateFloatBuffer_SGL};
    void (* const CreateUintBuffer_GS[])(unsigned int num, unsigned int* buffer, unsigned int& buf_id) = {CreateUintBuffer_GL, CreateUintBuffer_SGL};
    void (* const CreateVec4Buffer_GS[])(unsigned int num, float* buffer, unsigned int& buf_id) = {CreateVec4Buffer_GL, CreateVec4Buffer_SGL};
    void (* const CreateVec3Buffer_GS[])(unsigned int num, float* buffer, unsigned int& buf_id) = {CreateVec3Buffer_GL, CreateVec3Buffer_SGL};
    void (* const CreateVec2Buffer_GS[])(unsigned int num, float* buffer, unsigned int& buf_id) = {CreateVec2Buffer_GL, CreateVec2Buffer_SGL};
    void (* const BindBufferFloat_GS[])(unsigned int prg, const char* attrname, unsigned int bufidx) = {BindBufferFloat_GL, BindBufferFloat_SGL};
    void (* const BindBufferUint_GS[])(unsigned int prg, const char* attrname, unsigned int bufidx) = {BindBufferUint_GL, BindBufferUint_SGL};
    void (* const BindBufferVec4_GS[])(unsigned int prg, const char* attrname, unsigned int bufidx) = {BindBufferVec4_GL, BindBufferVec4_SGL};
    void (* const BindBufferVec3_GS[])(unsigned int prg, const char* attrname, unsigned int bufidx) = {BindBufferVec3_GL, BindBufferVec3_SGL};
    void (* const BindBufferVec2_GS[])(unsigned int prg, const char* attrname, unsigned int bufidx) = {BindBufferVec2_GL, BindBufferVec2_SGL};
    void (* const UnBindBuffer_GS[])(unsigned int prg, const char* attrname) = {UnBindBuffer_GL, UnBindBuffer_SGL};

    void (* const BindTexture2D_GS[])(unsigned int texid) = {BindTexture2D_GL, BindTexture2D_SGL};
    void (* const ActiveTexture_GS[])(unsigned int n) = {ActiveTexture_GL, ActiveTexture_SGL};
    
    void (* const TexImage2D_GS[])(unsigned int width, unsigned int height,
                                   unsigned int component, const unsigned char* pixeldata,
                                   bool filter, bool clampToEdgeS, bool clampToEdgeT) = {TexImage2D_GL, TexImage2D_SGL};
 
    void (* const TexImage2DFloat_GS[])(unsigned int width, unsigned int height,
                                        unsigned int component, const float* pixeldata,
                                        bool filter, bool clampToEdgeS, bool clampToEdgeT) = {TexImage2DFloat_GL, TexImage2DFloat_SGL};
 
    
    void (* const ReleaseBufferVBIB_GS[])(unsigned int buffer_id) = {ReleaseBufferVBIB_GL, ReleaseBufferVBIB_SGL};
    void (* const CreateVBIB_GS[])(unsigned int vertexnum, float* posbuffer, float* normalbuffer, float* matbuffer,
                                   float* texbuffer, unsigned int indexnum, unsigned int* indexbuffer,
                                   unsigned int& vtx_id, unsigned int& normal_id, unsigned int& mat_id,
                                   unsigned int& tex_id, unsigned int& index_id) = {CreateVBIB_GL, CreateVBIB_SGL};
    void (* const BindVBIB_GS[])(unsigned int prg, unsigned int vtxidx, unsigned int normalidx,
                                 unsigned int vtx_material, unsigned int texidx, unsigned int indexidx) = {BindVBIB_GL, BindVBIB_SGL};
    void (* const UnBindVBIB_GS[])(unsigned int prg) = {UnBindVBIB_GL, UnBindVBIB_SGL};
    void (* const DrawElements_GS[])(unsigned int indexnum) = {DrawElements_GL, DrawElements_SGL};
    void (* const DrawArrays_GS[])(unsigned int vtxnum) = {DrawArrays_GL, DrawArrays_SGL};
    
    void (* const GenTextures_GS[])(int n, unsigned int* tex) = {GenTextures_GL, GenTextures_SGL};

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
    void (* const BindPointVB_GS[])(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material) = {BindPointVB_GL, BindPointVB_SGL};
    void (* const UnBindPointVB_GS[])(unsigned int prg) = {UnBindPointVB_GL, UnBindPointVB_SGL};
    void (* const DrawPointArrays_GS[])(unsigned int vtxnum) = {DrawPointArrays_GL, DrawPointArrays_SGL};
    
    
    void (* const BindTetraVBIB_GS[])(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx) = {BindTetraVBIB_GL, BindTetraVBIB_SGL};
    void (* const UnBindTetraVBIB_GS[])(unsigned int prg) = {UnBindTetraVBIB_GL, UnBindTetraVBIB_SGL};
    void (* const DrawTetraArrays_GS[])(unsigned int vtxnum) = {DrawTetraArrays_GL, DrawTetraArrays_SGL};

}

/// コンストラクタ
BaseBuffer::BaseBuffer(RenderPlugin* render)
: m_render(render), m_prog(0), m_mode(render->GetRenderMode())
{
}

/// デストラクタ
BaseBuffer::~BaseBuffer()
{
}

/// シェーダプログラムをバインドする.
void BaseBuffer::BindProgram() const
{
    BindProgram_GS[m_mode](m_prog);
}
/**
 * Uniform変数の設定.
 * @param name 変数名.
 * @param val 値.
 */
void BaseBuffer::Uniform2fv(const char* name, const float* val) const
{
    SetUniform2fv_GS[m_mode](m_prog, name, val);
}
/**
 * Uniform変数の設定.
 * @param name 変数名.
 * @param val 値.
 */
void BaseBuffer::Uniform4fv(const char* name, const float* val) const
{
    SetUniform4fv_GS[m_mode](m_prog, name, val);
}
/**
 * カメラの設定.
 * @param camera カメラ
 */
void BaseBuffer::SetCamera(const Camera* camera) const
{
    const Camera::CameraInfo* info = camera->GetCameraInfo();
    
    // TODO:
    //if (camera->CameraType() == TYPE_STEREO) {
    //  SetStereoEnvCamera_SGL(prog, info->eye, info->tar, info->up, 20.0f, 0.03f); // fixme
    //else
    if (m_mode == RENDER_SURFACE) {
        SetCamera_SGL(m_prog, info->eye, info->tar, info->up, info->fov);
    } else {
        SetCamera_GL(m_prog, info->eye, info->tar, info->up, info->fov);//, 512, 512, 0.1f, 500.0f); // TEMP value
    }
}

void BaseBuffer::UnbindProgram() const
{
    //BindProgram_GS(0);
}

/**
 * Uniform変数のバインド.
 * @param obj レンダーオブジェクト
 */
void BaseBuffer::bindUniforms(const RenderObject* obj) const
{
    const unsigned int prg = getProgram();
    const VX::Math::matrix& mat = obj->GetTransformMatrix();
    SetUniformMatrix_GS[m_mode](prg, "lsgl_World", &mat.f[0]);

    // For a convenience, we add inverse and inverse transpose of world matrix to uniform shader variable.
    {
        VX::Math::matrix invmat = Inverse(mat);
        SetUniformMatrix_GS[m_mode](prg, "lsgl_WorldInverse", &invmat.f[0]);
    }

    {
        VX::Math::matrix ivtmat = Transpose(Inverse(mat));
        SetUniformMatrix_GS[m_mode](prg, "lsgl_WorldInverseTranspose", &ivtmat.f[0]);
    }

    
    
    const RenderObject::Vec4Map& vec4array = obj->GetUniformVec4();
    RenderObject::Vec4Map::const_iterator it4, eit4 = vec4array.end();
    for (it4 = vec4array.begin(); it4 != eit4; ++it4) {
        const VX::Math::vec4& v4 = it4->second;
        SetUniform4fv_GS[m_mode](prg, it4->first.c_str(), (const float*)&v4);
    }
    const RenderObject::Vec3Map& vec3array = obj->GetUniformVec3();
    RenderObject::Vec3Map::const_iterator it3, eit3 = vec3array.end();
    for (it3 = vec3array.begin(); it3 != eit3; ++it3) {
        const VX::Math::vec4& v3 = it3->second;
        SetUniform3fv_GS[m_mode](prg, it3->first.c_str(), (const float*)&v3);
    }
    const RenderObject::Vec2Map& vec2array = obj->GetUniformVec2();
    RenderObject::Vec2Map::const_iterator it2, eit2 = vec2array.end();
    for (it2 = vec2array.begin(); it2 != eit2; ++it2) {
        const VX::Math::vec4& v2 = it2->second;
        SetUniform2fv_GS[m_mode](prg, it2->first.c_str(), (const float*)&v2);
    }
    const RenderObject::FloatMap& floatarray = obj->GetUniformFloat();
    RenderObject::FloatMap::const_iterator itf, eitf = floatarray.end();
    for (itf = floatarray.begin(); itf != eitf; ++itf) {
        const float vf = itf->second;
        SetUniform1f_GS[m_mode](prg, itf->first.c_str(), vf);
    }
    const RenderObject::IntMap& intarray = obj->GetUniformInt();
    RenderObject::IntMap::const_iterator iti, eiti = intarray.end();
    for (iti = intarray.begin(); iti != eiti; ++iti) {
        const float vi = iti->second;
        SetUniform1i_GS[m_mode](prg, iti->first.c_str(), vi);
    }
    
    int textureCount = 1; // start from 1. 0 is default texture. Ex: volume texture.
    const RenderObject::TextureMap& texarray = obj->GetUniformTexture();
    RenderObject::TextureMap::const_iterator itt, eitt = texarray.end();
    for (itt = texarray.begin(); itt != eitt; ++itt) {
        const BufferImageData* vt = itt->second;
        const int texid = getTextureId(vt);
        if (texid > 0) {
            ActiveTexture_GS[m_mode](textureCount);
            BindTexture2D_GS[m_mode](texid);
            SetUniform1i_GS[m_mode](prg, itt->first.c_str(), textureCount);
            ++textureCount;
            ActiveTexture_GS[m_mode](0);
        }
    }

}

/**
 * 拡張バッファの作成.
 * @param obj レンダーオブジェクト
 */
void BaseBuffer::createExtraBuffers(const RenderObject* obj)
{
    const RenderObject::ExtraBufferMap& emap = obj->GetExtraBuffers();
    RenderObject::ExtraBufferMap::const_iterator it, eit = emap.end();
    for (it = emap.begin(); it != eit; ++it) {
        const RefPtr<BufferExtraData>& p = it->second;
        std::string dtype(p->GetDataType());
        unsigned int bufidx = 0;
        if (dtype == "float") {
            CreateFloatBuffer_GS[m_mode](p->GetNum(), p->Float()->GetBuffer(), bufidx);
        } else if (dtype == "vec4") {
            CreateVec4Buffer_GS[m_mode](p->GetNum(), p->Vec4()->GetBuffer(), bufidx);
        } else if (dtype == "vec3") {
            CreateVec3Buffer_GS[m_mode](p->GetNum(), p->Vec3()->GetBuffer(), bufidx);
        } else if (dtype == "vec2") {
            CreateVec2Buffer_GS[m_mode](p->GetNum(), p->Vec2()->GetBuffer(), bufidx);
        } else if (dtype == "uint") {
            CreateUintBuffer_GS[m_mode](p->GetNum(), p->Uint()->GetBuffer(), bufidx);
        }
        m_extraIdx[it->first] = bufidx;
    }
}

/**
 * 拡張バッファのバインド.
 * @param obj レンダーオブジェクト
 */
void BaseBuffer::bindExtraBuffers(const RenderObject* obj) const
{
    const unsigned int prg = getProgram();
    if (prg == 0) {
        return;
    }
    
    const RenderObject::ExtraBufferMap& emap = obj->GetExtraBuffers();
    RenderObject::ExtraBufferMap::const_iterator it, eit = emap.end();
    for (it = emap.begin(); it != eit; ++it) {
        const std::string& name = it->first;
        const RefPtr<BufferExtraData>& p = it->second;
        std::map<std::string, unsigned int>::const_iterator it = m_extraIdx.find(name);
        if (it == m_extraIdx.end())
            continue;
        const unsigned int bufidx = it->second;
        std::string dtype(p->GetDataType());
        if (dtype == "float") {
            BindBufferFloat_GS[m_mode](prg, name.c_str(), bufidx);
        } else if (dtype == "vec4") {
            BindBufferVec4_GS[m_mode](prg, name.c_str(), bufidx);
        } else if (dtype == "vec3") {
            BindBufferVec3_GS[m_mode](prg, name.c_str(), bufidx);
        } else if (dtype == "vec2") {
            BindBufferVec2_GS[m_mode](prg, name.c_str(), bufidx);
        } else if (dtype == "uint") {
            BindBufferUint_GS[m_mode](prg, name.c_str(), bufidx);
        }
    }
}

/**
 * 拡張バッファのアンバインド.
 * @param obj レンダーオブジェクト
 */
void BaseBuffer::unbindExtraBuffers(const RenderObject* obj) const
{
    const unsigned int prg = getProgram();
    if (prg == 0) {
        return;
    }
    
    const RenderObject::ExtraBufferMap& emap = obj->GetExtraBuffers();
    RenderObject::ExtraBufferMap::const_iterator it, eit = emap.end();
    for (it = emap.begin(); it != eit; ++it) {
        const std::string& name = it->first;
        const RefPtr<BufferExtraData>& p = it->second;
        std::map<std::string, unsigned int>::const_iterator it = m_extraIdx.find(name);
        if (it == m_extraIdx.end())
            continue;
        const unsigned int bufidx = it->second;
        std::string dtype(p->GetDataType());
        UnBindBuffer_GS[m_mode](prg, name.c_str());
    }
}
    
//-------------------------------------------------------------------

/**
 * シェーダソースの読み込み
 * @param srcname ソースファイルパス.
 */
bool BaseBuffer::loadShaderSrc(const char* srcname)
{
    m_prog = 0;
    return m_render->CreateProgramSrc(srcname, m_prog);
}

/// シェーダプログラムを返す.
unsigned int BaseBuffer::getProgram() const
{
    return m_prog;
}

/**
 * テクスチャIDの取得.
 * @param buf バッファイメージデータ
 * @retval テクスチャID
 */
unsigned int BaseBuffer::getTextureId(const BufferImageData* buf) const
{
    unsigned int tex = 0;
    bool haveTex = m_render->GetTexture(buf, tex);
    if (!haveTex) {
        fprintf(stderr, "Not Cached texture\n");
    }
    return tex;
}

/**
 * テクスチャをキャッシュする.
 * @param buf バッファイメージデータ
 */
bool BaseBuffer::cacheTexture(const BufferImageData *buf, bool filter, bool clampToEdgeS, bool clampToEdgeT)
{
    unsigned int tex;
    bool haveTex = m_render->GetTexture(buf, tex);
    if (!haveTex) {
        m_render->CreateTexture(buf, tex);
    }
    if (buf->IsNeedUpdate()) {
        buf->updated();
        BindTexture2D_GS[m_mode](tex);
        
        const BufferImageData::FORMAT fmt = buf->Format();
        
        assert(fmt == BufferImageData::RGBA8 || fmt == BufferImageData::RGBA32F || fmt == BufferImageData::R32F ); // TODO: SUPPORT others
        int component = 4;  // TODO: get size from buf->Format();
        
        // TODO: more format
        if (fmt == BufferImageData::RGBA8) {
            TexImage2D_GS[m_mode](buf->Width(), buf->Height(), component, buf->ImageBuffer()->GetBuffer(), filter, clampToEdgeS, clampToEdgeT);
        } else if (fmt == BufferImageData::RGBA32F) {
            TexImage2DFloat_GS[m_mode](buf->Width(), buf->Height(), component, buf->FloatImageBuffer()->GetBuffer(), filter, clampToEdgeS, clampToEdgeT);
        } else if (fmt == BufferImageData::R32F) {
            TexImage2DFloat_GS[m_mode](buf->Width(), buf->Height(), 1, buf->FloatImageBuffer()->GetBuffer(), filter, clampToEdgeS, clampToEdgeT);
        } else {
            assert(0);
        }        
    }
    return true;
}

/**
 * テクスチャをキャッシュする.
 * @param model レンダーオブジェクト
 */
void BaseBuffer::cacheTextures(const RenderObject* model)
{
    const RenderObject::TextureMap& tex = model->GetUniformTexture();
    const RenderObject::FilteringParamMap& filtering = model->GetTextureFiltering();
    const RenderObject::WrappingParamMap& wrapping = model->GetTextureWrapping();
    RenderObject::TextureMap::const_iterator it, eit = tex.end();
    for (it = tex.begin(); it != eit; ++it) {
        bool filter = true; // default: GL_LINEAR
        if (filtering.find(it->first) != filtering.end()) {
            RenderObject::FilteringParamMap::const_iterator itFilter = filtering.find(it->first);
            filter = itFilter->second;
        }
        bool clampToEdgeS = false;
        bool clampToEdgeT = false;
        bool clampToEdgeR = false;
        if (wrapping.find(it->first) != wrapping.end()) {
            RenderObject::WrappingParamMap::const_iterator itWrapping = wrapping.find(it->first);
            std::vector<bool> wrappings = itWrapping->second;
            if (wrappings.size() >= 2) {
                clampToEdgeS = wrappings[0];
                clampToEdgeT = wrappings[1];
            }
            if (wrappings.size() >= 3) {
                clampToEdgeR = wrappings[2];
            }
        }
        cacheTexture(it->second, filter, clampToEdgeS, clampToEdgeT);
    }
}

//-----------------------

void BaseBuffer::ReleaseBuffer(unsigned int bufferId) const {
    ReleaseBufferVBIB_GS[m_mode](bufferId);
}

void BaseBuffer::CreateVBIB(unsigned int vertexnum, float* posbuffer, float* normalbuffer, float* matbuffer,
                float* texbuffer, unsigned int indexnum, unsigned int* indexbuffer,
                unsigned int& vtx_id, unsigned int& normal_id, unsigned int& mat_id,
                unsigned int& tex_id, unsigned int& index_id) const
{
    CreateVBIB_GS[m_mode](vertexnum, posbuffer, normalbuffer, matbuffer,
                          texbuffer, indexnum, indexbuffer,
                          vtx_id, normal_id, mat_id,
                          tex_id, index_id);
    
}
void BaseBuffer::BindVBIB(unsigned int prg, unsigned int vtxidx, unsigned int normalidx,
              unsigned int vtx_material, unsigned int texidx, unsigned int indexidx) const
{
    BindVBIB_GS[m_mode](prg, vtxidx, normalidx, vtx_material, texidx, indexidx);
}
void BaseBuffer::UnBindVBIB(unsigned int prg) const
{
    UnBindVBIB_GS[m_mode](prg);
}
void BaseBuffer::DrawElements(unsigned int indexnum) const
{
    DrawElements_GS[m_mode](indexnum);
}
void BaseBuffer::DrawArrays(unsigned int vtxnum) const
{
    DrawArrays_GS[m_mode](vtxnum);
}

void BaseBuffer::GenTextures(int n, unsigned int* tex) const
{
    GenTextures_GS[m_mode](n, tex);
}

void BaseBuffer::BindTexture3D(unsigned int tex) const
{
    if (m_mode == RENDER_SURFACE)
        BindTexture3D_SGL(tex);
}

void BaseBuffer::TexImage3DPointer(unsigned int width, unsigned int height, unsigned int depth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR) const
{
    if (m_mode == RENDER_SURFACE)
        TexImage3DPointer_SGL(width, height, depth, component, volumedata, clampToEdgeS, clampToEdgeT, clampToEdgeR);
}

void BaseBuffer::SetUniform3fv(unsigned int prg, const char* name, const float* val) const
{
    SetUniform3fv_GS[m_mode](prg, name, val);
}
void BaseBuffer::SetUniform1i(unsigned int prg, const char* name, int val) const
{
    SetUniform1i_GS[m_mode](prg, name, val);
}

void BaseBuffer::TexCoordRemap3D(int axis, int n, const float* values) const
{
    if (m_mode == RENDER_SURFACE)
        TexCoordRemap3D_SGL(axis, n, values);
}

void BaseBuffer::BindLineVBIB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material, unsigned int indexidx) const {
    BindLineVBIB_GS[m_mode](prg, vtxidx, vtx_radius, vtx_material, indexidx);
}
void BaseBuffer::UnBindLineVBIB(unsigned int prg) const {
    UnBindLineVBIB_GS[m_mode](prg);
}
void BaseBuffer::DrawLineElements(unsigned int indexnum) const {
    DrawLineElements_GS[m_mode](indexnum);
}
void BaseBuffer::DrawLineArrays(unsigned int vtxnum) const {
    DrawLineArrays_GS[m_mode](vtxnum);
}
void BaseBuffer::CreateVBRM(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                                  unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id) const {
    CreateVBRM_GS[m_mode](vertexnum, posbuffer, radiusbuffer, matbuffer, vtx_id, radius_id, mat_id);
}
void BaseBuffer::CreateVBIBRM(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                              unsigned int indexnum, unsigned int* indexbuffer,
                              unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id, unsigned int& index_id)const
{
    CreateVBIBRM_GS[m_mode](vertexnum, posbuffer, radiusbuffer, matbuffer, indexnum, indexbuffer, vtx_id, radius_id, mat_id, index_id);
}

void BaseBuffer::LineWidth(float w) const {
    LineWidth_GS[m_mode](w);
}

void BaseBuffer::BindPointVB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material) const {
    BindPointVB_GS[m_mode](prg, vtxidx, vtx_radius, vtx_material);
}
void BaseBuffer::UnBindPointVB(unsigned int prg) const
{
    UnBindPointVB_GS[m_mode](prg);
}
void BaseBuffer::DrawPointArrays(unsigned int vtxnum) const
{
    DrawPointArrays_GS[m_mode](vtxnum);
}



void BaseBuffer::BindTetraVBIB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx) const
{
    BindTetraVBIB_GS[m_mode](prg, vtxidx, vtx_material, indexidx);
}
void BaseBuffer::UnBindTetraVBIB(unsigned int prg) const {
    UnBindTetraVBIB_GS[m_mode](prg);
}
void BaseBuffer::DrawTetraArrays(unsigned int vtxnum) const {
    DrawTetraArrays_GS[m_mode](vtxnum);
}

void BaseBuffer::BindSolidVBIB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx) const
{
    if (m_mode == RENDER_SURFACE){
        BindSolidVBIB_SGL(prg, vtxidx, vtx_material, indexidx);
    }
}
void BaseBuffer::UnBindSolidVBIB(unsigned int prg) const {
    if (m_mode == RENDER_SURFACE){
        UnBindSolidVBIB_SGL(prg);
    }
}

void BaseBuffer::DrawSolidArrays(int solidType, unsigned int vtxnum) const
{
    if (m_mode == RENDER_SURFACE)
        DrawSolidArrays_SGL(solidType, vtxnum);
}

void BaseBuffer::SparseTexImage3DPointer(int level, unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, unsigned int depth, unsigned int cellWidth, unsigned int cellHeight, unsigned int cellDepth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR) const
{
    if (m_mode == RENDER_SURFACE) {
        SparseTexImage3DPointer_SGL(level, xoffset, yoffset, zoffset, width, height, depth,
                                    cellWidth, cellHeight, cellDepth, component, volumedata,
                                    clampToEdgeS, clampToEdgeT, clampToEdgeR);
    }
}

