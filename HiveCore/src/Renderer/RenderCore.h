/**
 * @file RenderCore.h
 * hrenderコア機能部
 */

#pragma once

/*
    RenderCore.h
*/

#include "RenderMode.h"

class RenderObject;
class BufferImageData;

class RenderPlugin;

/**
 * Renderingコア機能部クラス
 */
class RenderCore
{
public:
    static RenderCore* GetInstance(RENDER_MODE mode = RENDER_SURFACE);
    static void Finalize();
    
    void AddRenderObject(RenderObject* robj);
    
    bool GetTexture(const BufferImageData* bufimg, unsigned int& id);
    bool CreateTexture(const BufferImageData* bufimg, unsigned int& tex);
    bool DeleteTexture(const BufferImageData* bufimg);

    void Render();
    void ClearRenderObject();
    
    void ClearBuffers(); // Clear RenderBuffer resouces
    void SetParallelRendering(bool enableParallel);

    bool CreateProgramSrc(const char* src, unsigned int& prog);
    
    void SetProgressCallback(bool (*func)(double));

    bool ClearShaderCache(const char* srcname);
    
private:
    RenderCore(RenderPlugin* render);
    ~RenderCore();

    RenderPlugin* m_imp;
};
