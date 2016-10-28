/**
 * @file BaseBuffer.h
 * ベースバッファ
 */
#ifndef __BASEBUFFER_H__
#define __BASEBUFFER_H__

/*
    BaseBuffer.h
 */

#include "RenderMode.h"
#include "../Core/Ref.h"

class Camera;
class RenderObject;
class BufferImageData;

#include <map>
#include <string>

/**
 * ベースバッファ
 */
class BaseBuffer : public RefCount
{
protected:
    BaseBuffer(RENDER_MODE mode);
    virtual ~BaseBuffer();
    
    RENDER_MODE m_mode;
    unsigned int m_prog;
    
    std::map<std::string, unsigned int> m_extraIdx;
    
public:
    virtual void Render() const = 0;
    virtual void Update() = 0;
    
    void BindProgram() const;
    void Uniform2fv(const char* name, const float*) const;
    void Uniform4fv(const char* name, const float*) const;
    void SetCamera(const Camera* camera) const;
    void UnbindProgram() const;
    
    //-------------------------------------------------------------------

protected:
    bool loadShaderSrc(const char* srcname);
    unsigned int getProgram() const;
    void bindUniforms(const RenderObject* obj) const;
    void createExtraBuffers(const RenderObject* obj);
    void bindExtraBuffers(const RenderObject* obj) const;
    unsigned int getTextureId(const BufferImageData* buf) const;
    bool cacheTexture(const BufferImageData* buf, bool filter, bool clampToEdgeS, bool clampToEdgeT);
    void cacheTextures(const RenderObject* model);
};

#endif // __BASEBUFFER_H__

