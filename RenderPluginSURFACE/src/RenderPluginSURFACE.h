/**
 * @file RenderPlugin.cpp
 * hrenderコア機能部
 */

#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include <Renderer/RenderPlugin.h>

class RenderPluginSURFACE : public RenderPlugin {

public:
    RenderPluginSURFACE();
    ~RenderPluginSURFACE();
    
    void ClearBuffers();
    void AddRenderObject(RenderObject* robj);
    void ClearRenderObject();
    void SetProgressCallback(bool (*func)(double));
    bool GetTexture(const BufferImageData* bufimg, unsigned int& id);
    bool CreateTexture(const BufferImageData* bufimg, unsigned int& tex);
    bool DeleteTexture(const BufferImageData* bufimg);
    bool CreateProgramSrc(const char* srcname, unsigned int& prg);
    bool ClearShaderCache(const char* srcname);
    void Render();

protected:
    // override virtual functions

    int GetWidth() const;
    int GetHeight() const;
    bool progressCallbackFunc(int progress, int y, int height) const;    

private:
    class Impl;
    Impl* m_imp;
};
