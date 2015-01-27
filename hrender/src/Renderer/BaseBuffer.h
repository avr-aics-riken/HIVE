#ifndef __BASEBUFFER_H__
#define __BASEBUFFER_H__

/*
    BaseBuffer.h
 */

#include "RenderMode.h"
#include "../Core/Ref.h"

class Camera;

class BaseBuffer : public RefCount
{
protected:
    BaseBuffer(RENDER_MODE mode);
    virtual ~BaseBuffer();
    
    RENDER_MODE m_mode;
    unsigned int m_prog;
    
public:
    virtual void Render() const = 0;
    
    void BindProgram() const;
    void Uniform2fv(const char* name, const float*) const;
    void Uniform4fv(const char* name, const float*) const;
    void SetCamera(const Camera* camera) const;
    void UnbindProgram() const;
    
    //-------------------------------------------------------------------

protected:
    bool loadShaderSrc(const char* srcname);
    unsigned int getProgram() const;
    
};

#endif // __BASEBUFFER_H__