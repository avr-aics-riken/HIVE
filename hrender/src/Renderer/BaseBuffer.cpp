
#include "BaseBuffer.h"
#include "../RenderObject/Camera.h"

#include "Commands.h"

BaseBuffer::BaseBuffer(RENDER_MODE mode) : m_mode(mode), m_prog(0)
{
}
BaseBuffer::~BaseBuffer()
{
    // delete -> m_prog
}

void BaseBuffer::BindProgram() const
{
    
}
void BaseBuffer::Uniform2fv(const char* name, const float*) const
{
}
void BaseBuffer::Uniform4fv(const char* name, const float*) const
{
    
}
void BaseBuffer::SetCamera(const Camera* camera) const
{
    const Camera::CameraInfo* info = camera->GetCameraInfo();
    
    // TODO:
    //if (camera->CameraType() == TYPE_STEREO) {
    //  SetStereoEnvCamera_SGL(prog, info->eye, info->tar, info->up, 20.0f, 0.03f); // fixme
    //else
    SetCamera_SGL(m_prog, info->eye, info->tar, info->up, info->fov);
}

void BaseBuffer::UnbindProgram() const
{
}
