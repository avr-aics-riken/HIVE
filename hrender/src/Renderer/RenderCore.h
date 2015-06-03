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

/**
 * hrenderコア機能部クラス
 */
class RenderCore
{
public:
    static RenderCore* GetInstance();
    
    void AddRenderObject(RenderObject* robj);
    
    bool GetTexture(const BufferImageData* bufimg, unsigned int& id);
    bool CreateTexture(const BufferImageData* bufimg, unsigned int& tex);
    bool DeleteTexture(const BufferImageData* bufimg);

    void Render();
    void ClearRenderObject();
    
    void ClearBuffers(); // Clear RenderBuffer resouces
    
    bool CreateProgramSrc(const char* src, unsigned int& prog);
    
    
    
    void SetProgressCallback(bool (*func)(double));

private:
    RenderCore();
    ~RenderCore();

    class Impl;
    Impl* m_imp;
};
