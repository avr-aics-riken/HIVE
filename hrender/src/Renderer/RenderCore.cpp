#include "RenderCore.h"

RenderCore* RenderCore::GetInstance()
{
    static RenderCore* inst = 0;
    if (inst)
        return inst;
    inst = new RenderCore();
    return inst;
}
    
RenderCore::RenderCore()
{
    
}

RenderCore::~RenderCore()
{
    
}

void RenderCore::AddRenderObject(RenderObject* robj)
{
    
}
void RenderCore::Render()
{
    
}
void RenderCore::ClearRenderObject()
{
    
}

