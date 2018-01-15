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

#include "../Renderer/RenderPlugin.h"

/// コンストラクタ
BaseBuffer::BaseBuffer(RenderPlugin* render)
: m_render(render), m_prog(0) {
}

/// デストラクタ
BaseBuffer::~BaseBuffer()
{
}

/// シェーダプログラムをバインドする.
void BaseBuffer::BindProgram() const
{
    m_render->BindProgram(m_prog);
}
/**
 * Uniform変数の設定.
 * @param name 変数名.
 * @param val 値.
 */
void BaseBuffer::Uniform2fv(const char* name, const float* val) const
{
    m_render->SetUniform2fv(m_prog, name, val);
}
/**
 * Uniform変数の設定.
 * @param name 変数名.
 * @param val 値.
 */
void BaseBuffer::Uniform4fv(const char* name, const float* val) const
{
    m_render->SetUniform4fv(m_prog, name, val);
}
/**
 * カメラの設定.
 * @param camera カメラ
 */
void BaseBuffer::SetCamera(const Camera* camera) const
{
    const Camera::CameraInfo* info = camera->GetCameraInfo();
    
    m_render->SetCamera(m_prog, info->eye, info->tar, info->up, info->fov);
    // TODO:
    //if (camera->CameraType() == TYPE_STEREO) {
    //  SetStereoEnvCamera_SGL(prog, info->eye, info->tar, info->up, 20.0f, 0.03f); // fixme
    //else
    
    /*if (m_mode == RENDER_SURFACE) {
        SetCamera_SGL(m_prog, info->eye, info->tar, info->up, info->fov);
    } else {
        SetCamera_GL(m_prog, info->eye, info->tar, info->up, info->fov);//, 512, 512, 0.1f, 500.0f); // TEMP value
    }*/
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
    m_render->SetUniformMatrix(prg, "lsgl_World", &mat.f[0]);

    // For a convenience, we add inverse and inverse transpose of world matrix to uniform shader variable.
    {
        VX::Math::matrix invmat = Inverse(mat);
        m_render->SetUniformMatrix(prg, "lsgl_WorldInverse", &invmat.f[0]);
    }

    {
        VX::Math::matrix ivtmat = Transpose(Inverse(mat));
        m_render->SetUniformMatrix(prg, "lsgl_WorldInverseTranspose", &ivtmat.f[0]);
    }

    
    
    const RenderObject::Vec4Map& vec4array = obj->GetUniformVec4();
    RenderObject::Vec4Map::const_iterator it4, eit4 = vec4array.end();
    for (it4 = vec4array.begin(); it4 != eit4; ++it4) {
        const VX::Math::vec4& v4 = it4->second;
        m_render->SetUniform4fv(prg, it4->first.c_str(), (const float*)&v4);
    }
    const RenderObject::Vec3Map& vec3array = obj->GetUniformVec3();
    RenderObject::Vec3Map::const_iterator it3, eit3 = vec3array.end();
    for (it3 = vec3array.begin(); it3 != eit3; ++it3) {
        const VX::Math::vec4& v3 = it3->second;
        m_render->SetUniform3fv(prg, it3->first.c_str(), (const float*)&v3);
    }
    const RenderObject::Vec2Map& vec2array = obj->GetUniformVec2();
    RenderObject::Vec2Map::const_iterator it2, eit2 = vec2array.end();
    for (it2 = vec2array.begin(); it2 != eit2; ++it2) {
        const VX::Math::vec4& v2 = it2->second;
        m_render->SetUniform2fv(prg, it2->first.c_str(), (const float*)&v2);
    }
    const RenderObject::FloatMap& floatarray = obj->GetUniformFloat();
    RenderObject::FloatMap::const_iterator itf, eitf = floatarray.end();
    for (itf = floatarray.begin(); itf != eitf; ++itf) {
        const float vf = itf->second;
        m_render->SetUniform1f(prg, itf->first.c_str(), vf);
    }
    const RenderObject::IntMap& intarray = obj->GetUniformInt();
    RenderObject::IntMap::const_iterator iti, eiti = intarray.end();
    for (iti = intarray.begin(); iti != eiti; ++iti) {
        const float vi = iti->second;
        m_render->SetUniform1i(prg, iti->first.c_str(), vi);
    }
    
    int textureCount = 1; // start from 1. 0 is default texture. Ex: volume texture.
    const RenderObject::TextureMap& texarray = obj->GetUniformTexture();
    RenderObject::TextureMap::const_iterator itt, eitt = texarray.end();
    for (itt = texarray.begin(); itt != eitt; ++itt) {
        const BufferImageData* vt = itt->second;
        const int texid = getTextureId(vt);
        if (texid > 0) {
            m_render->ActiveTexture(textureCount);
            m_render->BindTexture2D(texid);
            m_render->SetUniform1i(prg, itt->first.c_str(), textureCount);
            ++textureCount;
            m_render->ActiveTexture(0);
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
            m_render->CreateFloatBuffer(p->GetNum(), p->Float()->GetBuffer(), bufidx);
        } else if (dtype == "vec4") {
            m_render->CreateVec4Buffer(p->GetNum(), p->Vec4()->GetBuffer(), bufidx);
        } else if (dtype == "vec3") {
            m_render->CreateVec3Buffer(p->GetNum(), p->Vec3()->GetBuffer(), bufidx);
        } else if (dtype == "vec2") {
            m_render->CreateVec2Buffer(p->GetNum(), p->Vec2()->GetBuffer(), bufidx);
        } else if (dtype == "uint") {
            m_render->CreateUintBuffer(p->GetNum(), p->Uint()->GetBuffer(), bufidx);
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
            m_render->BindBufferFloat(prg, name.c_str(), bufidx);
        } else if (dtype == "vec4") {
            m_render->BindBufferVec4(prg, name.c_str(), bufidx);
        } else if (dtype == "vec3") {
            m_render->BindBufferVec3(prg, name.c_str(), bufidx);
        } else if (dtype == "vec2") {
            m_render->BindBufferVec2(prg, name.c_str(), bufidx);
        } else if (dtype == "uint") {
            m_render->BindBufferUint(prg, name.c_str(), bufidx);
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
        m_render->UnBindBuffer(prg, name.c_str());
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
        m_render->BindTexture2D(tex);
        
        const BufferImageData::FORMAT fmt = buf->Format();
        
        assert(fmt == BufferImageData::RGBA8 || fmt == BufferImageData::RGBA32F || fmt == BufferImageData::R32F ); // TODO: SUPPORT others
        int component = 4;  // TODO: get size from buf->Format();
        
        // TODO: more format
        if (fmt == BufferImageData::RGBA8) {
            m_render->TexImage2D(buf->Width(), buf->Height(), component, buf->ImageBuffer()->GetBuffer(), filter, clampToEdgeS, clampToEdgeT);
        } else if (fmt == BufferImageData::RGBA32F) {
            m_render->TexImage2DFloat(buf->Width(), buf->Height(), component, buf->FloatImageBuffer()->GetBuffer(), filter, clampToEdgeS, clampToEdgeT);
        } else if (fmt == BufferImageData::R32F) {
            m_render->TexImage2DFloat(buf->Width(), buf->Height(), 1, buf->FloatImageBuffer()->GetBuffer(), filter, clampToEdgeS, clampToEdgeT);
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
    m_render->ReleaseBuffer(bufferId);
}

void BaseBuffer::CreateVBIB(unsigned int vertexnum, float* posbuffer, float* normalbuffer, float* matbuffer,
                float* texbuffer, unsigned int indexnum, unsigned int* indexbuffer,
                unsigned int& vtx_id, unsigned int& normal_id, unsigned int& mat_id,
                unsigned int& tex_id, unsigned int& index_id) const
{
    m_render->CreateVBIB(vertexnum, posbuffer, normalbuffer, matbuffer,
                          texbuffer, indexnum, indexbuffer,
                          vtx_id, normal_id, mat_id,
                          tex_id, index_id);
    
}
void BaseBuffer::BindVBIB(unsigned int prg, unsigned int vtxidx, unsigned int normalidx,
              unsigned int vtx_material, unsigned int texidx, unsigned int indexidx) const
{
    m_render->BindVBIB(prg, vtxidx, normalidx, vtx_material, texidx, indexidx);
}
void BaseBuffer::UnBindVBIB(unsigned int prg) const
{
    m_render->UnBindVBIB(prg);
}
void BaseBuffer::DrawElements(unsigned int indexnum) const
{
    m_render->DrawElements(indexnum);
}
void BaseBuffer::DrawArrays(unsigned int vtxnum) const
{
    m_render->DrawArrays(vtxnum);
}

void BaseBuffer::GenTextures(int n, unsigned int* tex) const
{
    m_render->GenTextures(n, tex);
}

void BaseBuffer::BindTexture3D(unsigned int tex) const
{
    m_render->BindTexture3D(tex);
}

void BaseBuffer::TexImage3DPointer(unsigned int width, unsigned int height, unsigned int depth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR) const
{
    m_render->TexImage3DPointer(width, height, depth, component, volumedata, clampToEdgeS, clampToEdgeT, clampToEdgeR);
}

void BaseBuffer::SetUniform3fv(unsigned int prg, const char* name, const float* val) const
{
    m_render->SetUniform3fv(prg, name, val);
}
void BaseBuffer::SetUniform1i(unsigned int prg, const char* name, int val) const
{
    m_render->SetUniform1i(prg, name, val);
}

void BaseBuffer::TexCoordRemap3D(int axis, int n, const float* values) const
{
    m_render->TexCoordRemap3D(axis, n, values);
}

void BaseBuffer::BindLineVBIB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material, unsigned int indexidx) const {
    m_render->BindLineVBIB(prg, vtxidx, vtx_radius, vtx_material, indexidx);
}
void BaseBuffer::UnBindLineVBIB(unsigned int prg) const {
    m_render->UnBindLineVBIB(prg);
}
void BaseBuffer::DrawLineElements(unsigned int indexnum) const {
    m_render->DrawLineElements(indexnum);
}
void BaseBuffer::DrawLineArrays(unsigned int vtxnum) const {
    m_render->DrawLineArrays(vtxnum);
}
void BaseBuffer::CreateVBRM(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                                  unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id) const {
    m_render->CreateVBRM(vertexnum, posbuffer, radiusbuffer, matbuffer, vtx_id, radius_id, mat_id);
}
void BaseBuffer::CreateVBIBRM(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                              unsigned int indexnum, unsigned int* indexbuffer,
                              unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id, unsigned int& index_id)const
{
    m_render->CreateVBIBRM(vertexnum, posbuffer, radiusbuffer, matbuffer, indexnum, indexbuffer, vtx_id, radius_id, mat_id, index_id);
}

void BaseBuffer::LineWidth(float w) const {
    m_render->LineWidth(w);
}

void BaseBuffer::BindPointVB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material) const {
    m_render->BindPointVB(prg, vtxidx, vtx_radius, vtx_material);
}
void BaseBuffer::UnBindPointVB(unsigned int prg) const
{
    m_render->UnBindPointVB(prg);
}
void BaseBuffer::DrawPointArrays(unsigned int vtxnum) const
{
    m_render->DrawPointArrays(vtxnum);
}



void BaseBuffer::BindTetraVBIB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx) const
{
    m_render->BindTetraVBIB(prg, vtxidx, vtx_material, indexidx);
}
void BaseBuffer::UnBindTetraVBIB(unsigned int prg) const {
    m_render->UnBindTetraVBIB(prg);
}
void BaseBuffer::DrawTetraArrays(unsigned int vtxnum) const {
    m_render->DrawTetraArrays(vtxnum);
}

void BaseBuffer::BindSolidVBIB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx) const
{
    m_render->BindSolidVBIB(prg, vtxidx, vtx_material, indexidx);
}
void BaseBuffer::UnBindSolidVBIB(unsigned int prg) const {
    m_render->UnBindSolidVBIB(prg);
}

void BaseBuffer::DrawSolidArrays(int solidType, unsigned int vtxnum) const
{
    m_render->DrawSolidArrays(solidType, vtxnum);
}

void BaseBuffer::SparseTexImage3DPointer(int level, unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, unsigned int depth, unsigned int cellWidth, unsigned int cellHeight, unsigned int cellDepth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR) const
{
    m_render->SparseTexImage3DPointer(level, xoffset, yoffset, zoffset, width, height, depth,
                                    cellWidth, cellHeight, cellDepth, component, volumedata,
                                    clampToEdgeS, clampToEdgeT, clampToEdgeR);
}

