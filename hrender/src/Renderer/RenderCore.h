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

/**
 * hrenderコア機能部クラス
 */
class RenderCore
{
public:
    static RenderCore* GetInstance();
    
    void AddRenderObject(RenderObject* robj);
    void Render();
    void ClearRenderObject();
    
    void ClearBuffers(); // Clear RenderBuffer resouces
    
    void SetProgressCallback(bool (*func)(double));

private:
    RenderCore();
    ~RenderCore();

    class Impl;
    Impl* m_imp;
};
