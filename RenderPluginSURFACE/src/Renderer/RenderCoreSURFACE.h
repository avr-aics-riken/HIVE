/**
 * @file RenderCoreSURFACE.h
 * hrenderコア機能部
 */

#pragma once

/*
    RenderCoreSURFACE.h
*/

#include <Core/Ref.h>
#include <Renderer/RenderCore.h>

class RenderObject;
class BufferImageData;
class RenderDevice;

/**
 * Renderingコア機能部クラス
 */
class RenderCoreSURFACE : public RenderCore
{
public:
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
    
    // for compositor
    void EnableDepthComposition(bool enable);
    void SetCompositionMergeID(int mergeID);

    const char* GetRendererName();
    
    RenderCoreSURFACE(RenderDevice* render);
    
    
protected:
    ~RenderCoreSURFACE();

private:
    RenderDevice* m_imp;
};
