/**
 * @file RenderCoreSURFACE.cpp
 * hrenderコア機能部
 */

#include "RenderCoreSURFACE.h"
#include "RenderDevice.h"

/// コンストラクタ
RenderCoreSURFACE::RenderCoreSURFACE(RenderDevice* render) : m_imp(render) {}

/// デストラクタ
RenderCoreSURFACE::~RenderCoreSURFACE()  { delete m_imp; }

/// レンダーオブジェクトの追加
/// @param robj レンダーオブジェクト
void RenderCoreSURFACE::AddRenderObject(RenderObject* robj)
{
    m_imp->AddRenderObject(robj);
}

bool RenderCoreSURFACE::GetTexture(const BufferImageData* bufimg, unsigned int& id)
{
    return m_imp->GetTexture(bufimg, id);
}

bool RenderCoreSURFACE::CreateTexture(const BufferImageData* bufimg, unsigned int& tex)
{
    return m_imp->CreateTexture(bufimg, tex);
}

bool RenderCoreSURFACE::DeleteTexture(const BufferImageData* bufimg)
{
    return m_imp->DeleteTexture(bufimg);
}

/// レンダー
void RenderCoreSURFACE::Render()
{
    m_imp->Render();
}

/// レンダーオブジェクトのクリア
void RenderCoreSURFACE::ClearRenderObject()
{
    m_imp->ClearRenderObject();
}

/// レンダーバッファのクリア
void RenderCoreSURFACE::ClearBuffers()
{
    m_imp->ClearBuffers();
}

/// プログレスコールバックの設定
void RenderCoreSURFACE::SetProgressCallback(bool (*func)(double))
{
    m_imp->SetProgressCallback(func);
}

bool RenderCoreSURFACE::CreateProgramSrc(const char* src, unsigned int& prg)
{
    return m_imp->CreateProgramSrc(src, prg);
}

bool RenderCoreSURFACE::ClearShaderCache(const char* src)
{
    return m_imp->ClearShaderCache(src);
}


