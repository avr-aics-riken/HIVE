
#include <string>
#include "BaseBuffer.h"
#include "../RenderObject/Camera.h"

#include "Commands.h"

BaseBuffer::BaseBuffer(RENDER_MODE mode)
: m_mode(mode), m_prog(0)
{
}
BaseBuffer::~BaseBuffer()
{
    // delete -> m_prog
}


void BaseBuffer::BindProgram() const
{
    BindProgram_SGL(m_prog);
}
void BaseBuffer::Uniform2fv(const char* name, const float* val) const
{
    SetUniform2fv_SGL(m_prog, name, val);
}
void BaseBuffer::Uniform4fv(const char* name, const float* val) const
{
    SetUniform4fv_SGL(m_prog, name, val);
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
    //BindProgram_SGL(0);
}

//-------------------------------------------------------------------

bool BaseBuffer::loadShaderSrc(const char* srcname)
{
    return CreateProgramSrc_SGL(srcname, m_prog);
}

unsigned int BaseBuffer::getProgram() const
{
    return m_prog;
}

