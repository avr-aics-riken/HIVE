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

#include "../Renderer/RenderCore.h"

#include "Commands.h"

namespace {
    void (* const SetUniform4fv_GS[])(unsigned int prg, const char* name, const float* val) = {SetUniform4fv_GL, SetUniform4fv_SGL};
    void (* const SetUniform3fv_GS[])(unsigned int prg, const char* name, const float* val) = {SetUniform3fv_GL, SetUniform3fv_SGL};
    void (* const SetUniform2fv_GS[])(unsigned int prg, const char* name, const float* val) = {SetUniform2fv_GL, SetUniform2fv_SGL};
    void (* const SetUniform1f_GS[])(unsigned int prg, const char* name, float val) = {SetUniform1f_GL, SetUniform1f_SGL};
    void (* const BindProgram_GS[])(unsigned int prg) = {BindProgram_GL, BindProgram_SGL};
    void (* const SetUniformMatrix_GS[])(unsigned int prg, const char* name, const float* val) = {SetUniformMatrix_GL, SetUniformMatrix_SGL};
}

/// コンストラクタ
BaseBuffer::BaseBuffer(RENDER_MODE mode)
: m_mode(mode), m_prog(0)
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
    //BindProgram_SGL(0);
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
    
    int textureCount = 1; // start from 1. 0 is default texture. Ex: volume texture.
    const RenderObject::TextureMap& texarray = obj->GetUniformTexture();
    RenderObject::TextureMap::const_iterator itt, eitt = texarray.end();
    for (itt = texarray.begin(); itt != eitt; ++itt) {
        const BufferImageData* vt = itt->second;
        const int texid = getTextureId(vt);
        if (texid > 0) {
            ActiveTexture_SGL(textureCount);
            BindTexture2D_SGL(texid);
            SetUniform1i_SGL(prg, itt->first.c_str(), textureCount);
            ++textureCount;
            ActiveTexture_SGL(0);
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
            CreateFloatBuffer_SGL(p->GetNum(), p->Float()->GetBuffer(), bufidx);
        } else if (dtype == "vec4") {
            CreateVec4Buffer_SGL(p->GetNum(), p->Vec4()->GetBuffer(), bufidx);
        } else if (dtype == "vec3") {
            CreateVec3Buffer_SGL(p->GetNum(), p->Vec3()->GetBuffer(), bufidx);
        } else if (dtype == "vec2") {
            CreateVec2Buffer_SGL(p->GetNum(), p->Vec2()->GetBuffer(), bufidx);
        } else if (dtype == "uint") {
            CreateUintBuffer_SGL(p->GetNum(), p->Uint()->GetBuffer(), bufidx);
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
            BindBufferFloat_SGL(prg, name.c_str(), bufidx);
        } else if (dtype == "vec4") {
            BindBufferVec4_SGL(prg, name.c_str(), bufidx);
        } else if (dtype == "vec3") {
            BindBufferVec3_SGL(prg, name.c_str(), bufidx);
        } else if (dtype == "vec2") {
            BindBufferVec2_SGL(prg, name.c_str(), bufidx);
        } else if (dtype == "uint") {
            BindBufferUint_SGL(prg, name.c_str(), bufidx);
        }
    }
}

//-------------------------------------------------------------------

/**
 * シェーダソースの読み込み
 * @param srcname ソースファイルパス.
 */
bool BaseBuffer::loadShaderSrc(const char* srcname)
{
    RenderCore* core = RenderCore::GetInstance();
    m_prog = 0;
    return core->CreateProgramSrc(srcname, m_prog);
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
const unsigned int BaseBuffer::getTextureId(const BufferImageData* buf) const
{
    RenderCore* core = RenderCore::GetInstance();
    unsigned int tex = 0;
    bool haveTex = core->GetTexture(buf, tex);
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
    RenderCore* core = RenderCore::GetInstance();
    unsigned int tex;
    bool haveTex = core->GetTexture(buf, tex);
    if (!haveTex) {
        core->CreateTexture(buf, tex);
    }
    if (buf->IsNeedUpdate()) {
        buf->updated();
        BindTexture2D_SGL(tex);
        
        const BufferImageData::FORMAT fmt = buf->Format();
        
        assert(fmt == BufferImageData::RGBA8 || fmt == BufferImageData::RGBA32F || fmt == BufferImageData::R32F ); // TODO: SUPPORT others
        int component = 4;  // TODO: get size from buf->Format();
        
        // TODO: more format
        if (fmt == BufferImageData::RGBA8) {
            TexImage2D_SGL(buf->Width(), buf->Height(), component, buf->ImageBuffer()->GetBuffer(), filter, clampToEdgeS, clampToEdgeT);
        } else if (fmt == BufferImageData::RGBA32F) {
            TexImage2DFloat_SGL(buf->Width(), buf->Height(), component, buf->FloatImageBuffer()->GetBuffer(), filter, clampToEdgeS, clampToEdgeT);
        } else if (fmt == BufferImageData::R32F) {
            TexImage2DFloat_SGL(buf->Width(), buf->Height(), 1, buf->FloatImageBuffer()->GetBuffer(), filter, clampToEdgeS, clampToEdgeT);
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

