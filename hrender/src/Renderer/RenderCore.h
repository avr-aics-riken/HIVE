#pragma once

/*
    RenderCore.h
*/

enum RENDER_MODE{
    RENDER_OPENGL,
    RENDER_LSGL
};

class RenderObject;

class RenderCore
{
public:
    static RenderCore* GetInstance();
    
    void AddRenderObject(RenderObject* robj);
    void Render();
    void ClearRenderObject();
    
private:
    RenderCore();
    ~RenderCore();

    class Impl;
    Impl* m_imp;
};
