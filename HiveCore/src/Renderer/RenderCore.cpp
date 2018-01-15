/**
 * @file RenderCore.cpp
 * hrenderコア機能部
 */

#include "RenderCore.h"
#include "RenderPlugin.h"



// ----------------------------------------------------
// RenderCore
// ---------------

/*
/// インスタンスの取得
namespace {
    RenderCore* rc_inst = 0;
}

RenderCore* RenderCore::GetInstance(RENDER_MODE mode)
{
    if (!rc_inst) {
        rc_inst = new RenderCore(new RenderPlugin(mode));
    }
	return rc_inst;
}

void RenderCore::Finalize()
{
    delete rc_inst;
    rc_inst = 0;
}*/

/// コンストラクタ
RenderCore::RenderCore(RenderPlugin* render) : m_imp(render) {}
/// デストラクタ
RenderCore::~RenderCore()  { delete m_imp; }

/// レンダーオブジェクトの追加
/// @param robj レンダーオブジェクト
void RenderCore::AddRenderObject(RenderObject* robj)
{
    m_imp->AddRenderObject(robj);
}

bool RenderCore::GetTexture(const BufferImageData* bufimg, unsigned int& id)
{
    return m_imp->GetTexture(bufimg, id);
}

bool RenderCore::CreateTexture(const BufferImageData* bufimg, unsigned int& tex)
{
    return m_imp->CreateTexture(bufimg, tex);
}

bool RenderCore::DeleteTexture(const BufferImageData* bufimg)
{
    return m_imp->DeleteTexture(bufimg);
}


/// レンダー
void RenderCore::Render()
{
    m_imp->Render();
}

/// レンダーオブジェクトのクリア
void RenderCore::ClearRenderObject()
{
    m_imp->ClearRenderObject();
}

/// レンダーバッファのクリア
void RenderCore::ClearBuffers()
{
    m_imp->ClearBuffers();
}

void RenderCore::SetParallelRendering(bool enableParallel)
{
    m_imp->SetParallelRendering(enableParallel);
}

/// プログレスコールバックの設定
void RenderCore::SetProgressCallback(bool (*func)(double))
{
    m_imp->SetProgressCallback(func);
}

bool RenderCore::CreateProgramSrc(const char* src, unsigned int& prg)
{
    return m_imp->CreateProgramSrc(src, prg);
}

bool RenderCore::ClearShaderCache(const char* src)
{
    return m_imp->ClearShaderCache(src);
}


