/**
 * @file RenderCore.h
 * hrenderコア機能部
 */

#pragma once

/*
    RenderCore.h
*/

#include <Core/Ref.h>

class RenderObject;

/**
 * Renderingコア機能部 Interface
 */
class RenderCore : public RefCount
{
public:
    virtual void AddRenderObject(RenderObject* robj) = 0;
    virtual void Render() = 0;
    virtual void ClearRenderObject() = 0;
    
    virtual void ClearBuffers() = 0;
    virtual void SetProgressCallback(bool (*func)(double)) = 0;
    virtual bool ClearShaderCache(const char* srcname) = 0; // TODO: MOVE TO RenderPluginSURFACE
    
    virtual const char* GetRendererName() = 0;
protected:
    RenderCore() {};
    virtual ~RenderCore() {};

};
