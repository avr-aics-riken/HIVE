/**
 * @file RenderCoreKVS.h
 * KVS RendererPlugin core
 */

#pragma once

#include <Core/Ref.h>
#include <Renderer/RenderCore.h>

class RenderObject;
class BufferImageData;

/**
 * Renderingコア機能部クラス
 */
class RenderCoreKVS : public RenderCore
{
protected:
    ~RenderCoreKVS();

public:
    RenderCoreKVS();
    
    void AddRenderObject(RenderObject* robj);
    void Render();
    void ClearRenderObject();
    
    void ClearBuffers(); // Clear RenderBuffer resouces
    void SetProgressCallback(bool (*func)(double));
    bool ClearShaderCache(const char* srcname);
    
    const char* GetRendererName();
    
private:
    class Impl;
    Impl* m_imp;
};
