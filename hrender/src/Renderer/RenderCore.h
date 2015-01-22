/*
    RenderCore.h
*/


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

};
