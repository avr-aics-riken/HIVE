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

#include "Commands.h"

/// コンストラクタ
BaseBuffer::BaseBuffer(RENDER_MODE mode)
: m_mode(mode), m_prog(0)
{
}

/// デストラクタ
BaseBuffer::~BaseBuffer()
{
    std::map<const BufferImageData*, unsigned int>::const_iterator it, eit = m_texutecache.end();
    for (it = m_texutecache.begin(); it != eit; ++it) {
        unsigned int t = it->second;
        DeleteTextures_SGL(1, &t);
    }
    DeleteProgram_SGL(m_prog);
}

/// シェーダプログラムをバインドする.
void BaseBuffer::BindProgram() const
{
    BindProgram_SGL(m_prog);
}
/**
 * Uniform変数の設定.
 * @param name 変数名.
 * @param val 値.
 */
void BaseBuffer::Uniform2fv(const char* name, const float* val) const
{
    SetUniform2fv_SGL(m_prog, name, val);
}
/**
 * Uniform変数の設定.
 * @param name 変数名.
 * @param val 値.
 */
void BaseBuffer::Uniform4fv(const char* name, const float* val) const
{
    SetUniform4fv_SGL(m_prog, name, val);
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
    SetCamera_SGL(m_prog, info->eye, info->tar, info->up, info->fov);
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
    SetUniformMatrix_SGL(prg, "lsgl_World", &mat.f[0]);

    // For a convenience, we add inverse and inverse transpose of world matrix to uniform shader variable.
    {
        VX::Math::matrix invmat = Inverse(mat);
        SetUniformMatrix_SGL(prg, "lsgl_WorldInverse", &invmat.f[0]);
    }

    {
        VX::Math::matrix ivtmat = Transpose(Inverse(mat));
        SetUniformMatrix_SGL(prg, "lsgl_WorldInverseTranspose", &ivtmat.f[0]);
    }

    
    
    const RenderObject::Vec4Map& vec4array = obj->GetUniformVec4();
    RenderObject::Vec4Map::const_iterator it4, eit4 = vec4array.end();
    for (it4 = vec4array.begin(); it4 != eit4; ++it4) {
        const VX::Math::vec4& v4 = it4->second;
        SetUniform4fv_SGL(prg, it4->first.c_str(), (const float*)&v4);
    }
    const RenderObject::Vec3Map& vec3array = obj->GetUniformVec3();
    RenderObject::Vec3Map::const_iterator it3, eit3 = vec3array.end();
    for (it3 = vec3array.begin(); it3 != eit3; ++it3) {
        const VX::Math::vec4& v3 = it3->second;
        SetUniform3fv_SGL(prg, it3->first.c_str(), (const float*)&v3);
    }
    const RenderObject::Vec2Map& vec2array = obj->GetUniformVec2();
    RenderObject::Vec2Map::const_iterator it2, eit2 = vec2array.end();
    for (it2 = vec2array.begin(); it2 != eit2; ++it2) {
        const VX::Math::vec4& v2 = it2->second;
        SetUniform2fv_SGL(prg, it2->first.c_str(), (const float*)&v2);
    }
    const RenderObject::FloatMap& floatarray = obj->GetUniformFloat();
    RenderObject::FloatMap::const_iterator itf, eitf = floatarray.end();
    for (itf = floatarray.begin(); itf != eitf; ++itf) {
        const float vf = itf->second;
        SetUniform1f_SGL(prg, itf->first.c_str(), vf);
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

//-------------------------------------------------------------------

/**
 * シェーダソースの読み込み
 * @param srcname ソースファイルパス.
 */
bool BaseBuffer::loadShaderSrc(const char* srcname)
{
    return CreateProgramSrc_SGL(srcname, m_prog);
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
    std::map<const BufferImageData*, unsigned int>::const_iterator it = m_texutecache.find(buf);
    if (it == m_texutecache.end()) {
        return 0;
    }
    return it->second;
}

/**
 * テクスチャをキャッシュする.
 * @param buf バッファイメージデータ
 */
bool BaseBuffer::cacheTexture(const BufferImageData *buf, bool filter)
{
    std::map<const BufferImageData*, unsigned int>::iterator it = m_texutecache.find(buf);
    if (buf->IsNeedUpdate()) {
        if (it != m_texutecache.end()) {
            m_texutecache.erase(it);
            it = m_texutecache.end();
        }
    }
    if (it != m_texutecache.end()) {
        return true;
    } else {
        buf->updated();
        unsigned int tex;
        GenTextures_SGL(1, &tex);
        BindTexture2D_SGL(tex);
        
        const BufferImageData::FORMAT fmt = buf->Format();
        
        assert(fmt == BufferImageData::RGBA8 || fmt == BufferImageData::RGBA32F || fmt == BufferImageData::R32F ); // TODO: SUPPORT others
        int component = 4;  // TODO: get size from buf->Format();
        
        // TODO: more format
        if (fmt == BufferImageData::RGBA8) {
            TexImage2D_SGL(buf->Width(), buf->Height(), component, buf->ImageBuffer()->GetBuffer(), filter);
        } else if (fmt == BufferImageData::RGBA32F) {
            TexImage2DFloat_SGL(buf->Width(), buf->Height(), component, buf->FloatImageBuffer()->GetBuffer(), filter);
        } else if (fmt == BufferImageData::R32F) {
            TexImage2DFloat_SGL(buf->Width(), buf->Height(), 1, buf->FloatImageBuffer()->GetBuffer(), filter);
        } else {
            assert(0);
        }
        
        m_texutecache[buf] = tex; //cache
        return true;
    }
}

/**
 * テクスチャをキャッシュする.
 * @param model レンダーオブジェクト
 */
void BaseBuffer::cacheTextures(const RenderObject* model)
{
    const RenderObject::TextureMap& tex = model->GetUniformTexture();
    const RenderObject::FilteringParamMap& filtering = model->GetTextureFiltering();
    RenderObject::TextureMap::const_iterator it, eit = tex.end();
    for (it = tex.begin(); it != eit; ++it) {
        bool filter = true; // default: GL_LINEAR
        if (filtering.find(it->first) != filtering.end()) {
            RenderObject::FilteringParamMap::const_iterator itFilter = filtering.find(it->first);
            filter = itFilter->second;
        }
        cacheTexture(it->second, filter);
    }
}

