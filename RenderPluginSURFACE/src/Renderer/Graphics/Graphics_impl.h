/*
 * Graphics_impl.h
 *
 * 低レベルグラフィックスライブラリ
 * Created by kioku on 11/07/06.
 *
 */

// !!! This is included by Graphics.h !!!
#ifndef INCLUDE_VXGRAPHICS_H
#error "You must include from Graphics.h. Do not include this file."
#else

#if defined(USE_LSGL)
#include <GLES2/gl2.h>
namespace {
	void InitGLExtention(){}
}
#elif defined(_WIN32)
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "WGLExtention.h"
#elif defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
namespace {
	void InitGLExtention(){}
}
#else // Linux
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
namespace {
	void InitGLExtention(){}
}
#endif

#include <assert.h>

#ifdef USE_GLEXTFUNC
#define glBindFramebuffer                     glBindFramebufferEXT
#define glBindRenderbuffer                    glBindRenderbufferEXT
#define glCheckFramebufferStatus              glCheckFramebufferStatusEXT
#define glDeleteFramebuffers                  glDeleteFramebuffersEXT
#define glDeleteRenderbuffers                 glDeleteRenderbuffersEXT
#define glFramebufferRenderbuffer             glFramebufferRenderbufferEXT
#define glFramebufferTexture2D                glFramebufferTexture2DEXT
#define glGenerateMipmap                      glGenerateMipmapEXT
#define glGenFramebuffers                     glGenFramebuffersEXT
#define glGenRenderbuffers                    glGenRenderbuffersEXT
#define glGetFramebufferAttachmentParameteriv glGetFramebufferAttachmentParameterivEXT
#define glGetRenderbufferParameteriv          glGetRenderbufferParameterivEXT
#define glIsFramebuffer                       glIsFramebufferEXT
#define glIsRenderbuffer                      glIsRenderbufferEXT
#define glRenderbufferStorage                 glRenderbufferStorageEXT
#define glBlendFuncSeparate                   glBlendFuncSeparateEXT
#endif


//#define GL_DEBUG

#ifdef GL_DEBUG
#define GLDEBUG	GL_debug_func(this, __LINE__) 
#include <assert.h>
#include <stdio.h>
#else

#define GLDEBUG 

#endif

namespace VX {

#ifdef GL_DEBUG
inline void GL_debug_func(Graphics* g, int line)
{
	VGenum e = g->GetError();
	if(e)
	{
#if defined(_WIN32)
		char buf[1024];
		sprintf(buf, "GL ERROR(0x%X):Line(%d) - %s\n", e, line, gluErrorString(e));
		OutputDebugStringA(buf);
#else
		printf("GL ERROR(0x%X):Line(%d) - %s\n", e, line, gluErrorString(e));
#endif
		assert(0);
	}
}
#endif
	
inline unsigned int getInternalFormatSize(VGenum internalformat)
{
	if     (internalformat == VG_LUMINANCE)  return 1;
	else if(internalformat == VG_RGB565)     return 2;
	else if(internalformat == VG_RGBA)       return 4;
	else if(internalformat == VG_BGRA)       return 4;
	else if(internalformat == VG_RGB)        return 3;
	else return 0;
}

		
inline Graphics::Graphics()
{
	InitGLExtention();
}

inline Graphics::~Graphics()
{
}
	
inline void Graphics::ActiveTexture (VGenum texture){ glActiveTexture(texture); GLDEBUG; }
inline void Graphics::AttachShader (unsigned int program, unsigned int shader){ glAttachShader(program, shader); GLDEBUG; }
inline void Graphics::BindAttribLocation (unsigned int program, unsigned int index, const char* name){ glBindAttribLocation(program, index, name); GLDEBUG; }
inline void Graphics::BindBuffer (VGenum target, unsigned int buffer){ glBindBuffer(target, buffer); GLDEBUG;}
inline void Graphics::BindFramebuffer (VGenum target, unsigned int framebuffer){ glBindFramebuffer(target, framebuffer); GLDEBUG; }
inline void Graphics::BindRenderbuffer (VGenum target, unsigned int renderbuffer){ glBindRenderbuffer(target, renderbuffer); GLDEBUG; }
inline void Graphics::BindTexture (VGenum target, unsigned int texture){ glBindTexture(target, texture); GLDEBUG;}
inline void Graphics::BlendColor (float red, float green, float blue, float alpha){ glBlendColor(red, green, blue, alpha); GLDEBUG; }
inline void Graphics::BlendEquation ( VGenum mode ){ glBlendEquation(mode); GLDEBUG; }
inline void Graphics::BlendEquationSeparate (VGenum modeRGB, VGenum modeAlpha){ glBlendEquationSeparate(modeRGB, modeAlpha); GLDEBUG; }
inline void Graphics::BlendFunc (VGenum sfactor, VGenum dfactor){ glBlendFunc(sfactor, dfactor); GLDEBUG; }
inline void Graphics::BlendFuncSeparate (VGenum srcRGB, VGenum dstRGB, VGenum srcAlpha, VGenum dstAlpha){ glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha); GLDEBUG; }
inline void Graphics::BufferData (VGenum target, int size, const void* data, VGenum usage){ glBufferData(target, size, data, usage); GLDEBUG;}
inline void Graphics::BufferSubData (VGenum target, int offset, int size, const void* data){ glBufferSubData(target, offset, size, data); GLDEBUG;}
inline VGenum Graphics::CheckFramebufferStatus (VGenum target){ return glCheckFramebufferStatus(target); GLDEBUG; }
inline void Graphics::Clear (unsigned int mask){ glClear(mask); GLDEBUG; }
inline void Graphics::ClearColor (float red, float green, float blue, float alpha){ glClearColor(red, green, blue, alpha); GLDEBUG; }
inline void Graphics::ClearDepthf (float depth){ assert(0); }//glClearDepthf(depth); GLDEBUG; }
inline void Graphics::ClearStencil (int s){ glClearStencil(s); GLDEBUG; }
inline void Graphics::ColorMask (unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha){ glColorMask(red, green, blue, alpha); GLDEBUG; }
inline void Graphics::CompileShader (unsigned int shader){ glCompileShader(shader); GLDEBUG; }
inline void Graphics::CompressedTexImage2D (VGenum target, int level, VGenum internalformat, int width, int height, int border, int imageSize, const void* data){ glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data); GLDEBUG; }
inline void Graphics::CompressedTexSubImage2D (VGenum target, int level, int xoffset, int yoffset, int width, int height, VGenum format, int imageSize, const void* data){ glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data); GLDEBUG; }
inline void Graphics::CopyTexImage2D (VGenum target, int level, VGenum internalformat, int x, int y, int width, int height, int border){ glCopyTexImage2D(target, level, internalformat, x, y, width, height, border); GLDEBUG; }
inline void Graphics::CopyTexSubImage2D (VGenum target, int level, int xoffset, int yoffset, int x, int y, int width, int height){ glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height); GLDEBUG; }
inline unsigned int  Graphics::CreateProgram (void){ return glCreateProgram(); GLDEBUG;}
inline unsigned int  Graphics::CreateShader (VGenum type){ return glCreateShader(type); GLDEBUG; }
inline void Graphics::CullFace (VGenum mode){ glCullFace(mode); GLDEBUG; }
inline void Graphics::DeleteBuffers (int n, const unsigned int* buffers){
	glDeleteBuffers(n, buffers); GLDEBUG;
}
inline void Graphics::DeleteFramebuffers (int n, const unsigned int* framebuffers){ glDeleteFramebuffers(n, framebuffers); GLDEBUG; }
inline void Graphics::DeleteProgram (unsigned int program){ glDeleteProgram(program); GLDEBUG;
}
inline void Graphics::DeleteRenderbuffers (int n, const unsigned int* renderbuffers){ glDeleteRenderbuffers(n, renderbuffers); GLDEBUG; }
inline void Graphics::DeleteShader (unsigned int shader){ glDeleteShader(shader); GLDEBUG; }
inline void Graphics::DeleteTextures (int n, const unsigned int* textures){ glDeleteTextures(n, textures); GLDEBUG;
}
inline void Graphics::DepthFunc (VGenum func){ glDepthFunc(func); GLDEBUG; }
inline void Graphics::DepthMask (unsigned char flag){ glDepthMask(flag); GLDEBUG; }
inline void Graphics::DepthRangef (float zNear, float zFar){ assert(0); }//glDepthRangef(zNear, zFar); GLDEBUG; }
inline void Graphics::DetachShader (unsigned int program, unsigned int shader){ glDetachShader(program, shader); GLDEBUG; }
inline void Graphics::Disable (VGenum cap){ glDisable(cap); GLDEBUG; }
inline void Graphics::DisableVertexAttribArray (unsigned int index){ glDisableVertexAttribArray(index); GLDEBUG; }
inline void Graphics::DrawArrays (VGenum mode, int first, int count){ glDrawArrays(mode, first, count); GLDEBUG;
}
inline void Graphics::DrawElements (VGenum mode, int count, VGenum type, const void* indices){ glDrawElements(mode, count, type, indices); GLDEBUG;
}
inline void Graphics::Enable (VGenum cap){ glEnable(cap); GLDEBUG; }
inline void Graphics::EnableVertexAttribArray (unsigned int index){ glEnableVertexAttribArray(index); GLDEBUG; }
inline void Graphics::Finish (void){ glFinish(); GLDEBUG; }
inline void Graphics::Flush (void){ glFlush(); GLDEBUG; }
inline void Graphics::FramebufferRenderbuffer (VGenum target, VGenum attachment, VGenum renderbuffertarget, unsigned int renderbuffer){ glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer); GLDEBUG; }
inline void Graphics::FramebufferTexture2D (VGenum target, VGenum attachment, VGenum textarget, unsigned int texture, int level){ glFramebufferTexture2D(target, attachment, textarget, texture, level); GLDEBUG; }
inline void Graphics::FrontFace (VGenum mode){ glFrontFace(mode); GLDEBUG; }
inline void Graphics::GenBuffers (int n, unsigned int* buffers){ glGenBuffers(n, buffers); GLDEBUG; }
inline void Graphics::GenerateMipmap (VGenum target){ glGenerateMipmap(target); GLDEBUG; }
inline void Graphics::GenFramebuffers (int n, unsigned int* framebuffers){ glGenFramebuffers(n, framebuffers); GLDEBUG; }
inline void Graphics::GenRenderbuffers (int n, unsigned int* renderbuffers){ glGenRenderbuffers(n, renderbuffers); GLDEBUG; }
inline void Graphics::GenTextures (int n, unsigned int* textures){ glGenTextures(n, textures); GLDEBUG; }
inline void Graphics::GetActiveAttrib (unsigned int program, unsigned int index, int bufsize, int* length, int* size, VGenum* type, char* name){ glGetActiveAttrib(program, index, bufsize, length, size, type, name); GLDEBUG; }
inline void Graphics::GetActiveUniform (unsigned int program, unsigned int index, int bufsize, int* length, int* size, VGenum* type, char* name){ glGetActiveUniform(program, index, bufsize, length, size, type, name); GLDEBUG; }
inline void Graphics::GetAttachedShaders (unsigned int program, int maxcount, int* count, unsigned int* shaders){ glGetAttachedShaders(program, maxcount, count, shaders); GLDEBUG; }
inline int  Graphics::GetAttribLocation (unsigned int program, const char* name){ return glGetAttribLocation(program, name); GLDEBUG; }
inline void Graphics::GetBooleanv (VGenum pname, unsigned char* params){ glGetBooleanv(pname, params); GLDEBUG; }
inline void Graphics::GetBufferParameteriv (VGenum target, VGenum pname, int* params){ glGetBufferParameteriv(target, pname, params); GLDEBUG; }
inline VGenum Graphics::GetError (void){ return glGetError(); }
inline void Graphics::GetFloatv (VGenum pname, float* params){ glGetFloatv(pname, params); GLDEBUG; }
inline void Graphics::GetFramebufferAttachmentParameteriv (VGenum target, VGenum attachment, VGenum pname, int* params){ glGetFramebufferAttachmentParameteriv(target, attachment, pname, params); GLDEBUG; }
inline void Graphics::GetIntegerv (VGenum pname, int* params){ glGetIntegerv(pname, params); GLDEBUG; }
inline void Graphics::GetProgramiv (unsigned int program, VGenum pname, int* params){ glGetProgramiv(program, pname, params); GLDEBUG; }
inline void Graphics::GetProgramInfoLog (unsigned int program, int bufsize, int* length, char* infolog){ glGetProgramInfoLog(program, bufsize, length, infolog); GLDEBUG; }
inline void Graphics::GetRenderbufferParameteriv (VGenum target, VGenum pname, int* params){ glGetRenderbufferParameteriv(target, pname, params); GLDEBUG; }
inline void Graphics::GetShaderiv (unsigned int shader, VGenum pname, int* params){ glGetShaderiv(shader, pname, params); GLDEBUG; }
inline void Graphics::GetShaderInfoLog (unsigned int shader, int bufsize, int* length, char* infolog){ glGetShaderInfoLog(shader, bufsize, length, infolog); GLDEBUG; }
inline void Graphics::GetShaderPrecisionFormat (VGenum shadertype, VGenum precisiontype, int* range, int* precision){ assert(0); }//glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision); GLDEBUG; }
inline void Graphics::GetShaderSource (unsigned int shader, int bufsize, int* length, char* source){ glGetShaderSource(shader, bufsize, length, source); GLDEBUG; }
inline const unsigned char* Graphics::GetString (VGenum name){ return glGetString(name); }
inline void Graphics::GetTexParameterfv (VGenum target, VGenum pname, float* params){ glGetTexParameterfv(target, pname, params); GLDEBUG; }
inline void Graphics::GetTexParameteriv (VGenum target, VGenum pname, int* params){ glGetTexParameteriv(target, pname, params); GLDEBUG; }
inline void Graphics::GetUniformfv (unsigned int program, int location, float* params){ glGetUniformfv(program, location, params); GLDEBUG; }
inline void Graphics::GetUniformiv (unsigned int program, int location, int* params){ glGetUniformiv(program, location, params); GLDEBUG; }
inline int  Graphics::GetUniformLocation (unsigned int program, const char* name){ return glGetUniformLocation(program, name); GLDEBUG; }
inline void Graphics::GetVertexAttribfv (unsigned int index, VGenum pname, float* params){ glGetVertexAttribfv(index, pname, params); GLDEBUG; }
inline void Graphics::GetVertexAttribiv (unsigned int index, VGenum pname, int* params){ glGetVertexAttribiv(index, pname, params); GLDEBUG; }
inline void Graphics::GetVertexAttribPointerv (unsigned int index, VGenum pname, void** pointer){ glGetVertexAttribPointerv(index, pname, pointer); GLDEBUG; }
inline void Graphics::Hint (VGenum target, VGenum mode){ glHint(target, mode); GLDEBUG; }
inline unsigned char   Graphics::IsBuffer (unsigned int buffer){ return glIsBuffer(buffer); GLDEBUG; }
inline unsigned char   Graphics::IsEnabled (VGenum cap){ return glIsEnabled(cap); GLDEBUG; }
inline unsigned char   Graphics::IsFramebuffer (unsigned int framebuffer){ return glIsFramebuffer(framebuffer); GLDEBUG; }
inline unsigned char   Graphics::IsProgram (unsigned int program){ return glIsProgram(program); GLDEBUG; }
inline unsigned char   Graphics::IsRenderbuffer (unsigned int renderbuffer){ return glIsRenderbuffer(renderbuffer); GLDEBUG; }
inline unsigned char   Graphics::IsShader (unsigned int shader){ return glIsShader(shader); GLDEBUG; }
inline unsigned char   Graphics::IsTexture (unsigned int texture){ return glIsTexture(texture); GLDEBUG; }
inline void Graphics::LineWidth (float width){ glLineWidth(width); GLDEBUG; }
inline void Graphics::LinkProgram (unsigned int program){ glLinkProgram(program); GLDEBUG; }
inline void Graphics::PixelStorei (VGenum pname, int param){ glPixelStorei(pname, param); GLDEBUG; }
inline void Graphics::PolygonOffset (float factor, float units){ glPolygonOffset(factor, units); GLDEBUG; }
inline void Graphics::ReadPixels (int x, int y, int width, int height, VGenum format, VGenum type, void* pixels){ glReadPixels(x, y, width, height, format, type, pixels); GLDEBUG; }
inline void Graphics::ReleaseShaderCompiler (void){ assert(0); }//glReleaseShaderCompiler(); GLDEBUG; }
inline void Graphics::RenderbufferStorage (VGenum target, VGenum internalformat, int width, int height){ glRenderbufferStorage(target, internalformat, width, height); GLDEBUG; }
inline void Graphics::SampleCoverage (float value, unsigned char invert){ glSampleCoverage(value, invert); GLDEBUG; }
inline void Graphics::Scissor (int x, int y, int width, int height){ glScissor(x, y, width, height); GLDEBUG; }
inline void Graphics::ShaderBinary (int n, const unsigned int* shaders, VGenum binaryformat, const void* binary, int length){ assert(0); }//glShaderBinary(n, shaders, binaryformat, binary, length); GLDEBUG; }
inline void Graphics::ShaderSource (unsigned int shader, int count, const char** string, const int* length){ glShaderSource(shader, count, string, length); GLDEBUG; }
inline void Graphics::StencilFunc (VGenum func, int ref, unsigned int mask){ glStencilFunc(func, ref, mask); GLDEBUG; }
inline void Graphics::StencilFuncSeparate (VGenum face, VGenum func, int ref, unsigned int mask){ glStencilFuncSeparate(face, func, ref, mask); GLDEBUG; }
inline void Graphics::StencilMask (unsigned int mask){ glStencilMask(mask); GLDEBUG; }
inline void Graphics::StencilMaskSeparate (VGenum face, unsigned int mask){ glStencilMaskSeparate(face, mask); GLDEBUG; }
inline void Graphics::StencilOp (VGenum fail, VGenum zfail, VGenum zpass){ glStencilOp(fail, zfail, zpass); GLDEBUG; }
inline void Graphics::StencilOpSeparate (VGenum face, VGenum fail, VGenum zfail, VGenum zpass){ glStencilOpSeparate(face, fail, zfail, zpass); GLDEBUG; }
inline void Graphics::TexImage2D (VGenum target, int level, int internalformat, int width, int height, int border, VGenum format, VGenum type, const void* pixels){ glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels); GLDEBUG;
}
inline void Graphics::TexParameterf  (VGenum target, VGenum pname, float param){ glTexParameterf(target, pname, param); GLDEBUG; }
inline void Graphics::TexParameterfv (VGenum target, VGenum pname, const float* params){ glTexParameterfv(target, pname, params); GLDEBUG; }
inline void Graphics::TexParameteri  (VGenum target, VGenum pname, int param){ glTexParameteri(target, pname, param); GLDEBUG; }
inline void Graphics::TexParameteriv (VGenum target, VGenum pname, const int* params){ glTexParameteriv(target, pname, params); GLDEBUG; }
inline void Graphics::TexSubImage2D  (VGenum target, int level, int xoffset, int yoffset, int width, int height, VGenum format, VGenum type, const void* pixels){ glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels); GLDEBUG;}
inline void Graphics::Uniform1f  (int location, float x){ glUniform1f(location, x); GLDEBUG; }
inline void Graphics::Uniform1fv (int location, int count, const float* v){ glUniform1fv(location, count, v); GLDEBUG; }
inline void Graphics::Uniform1i  (int location, int x){ glUniform1i(location, x); GLDEBUG; }
inline void Graphics::Uniform1iv (int location, int count, const int* v){ glUniform1iv(location, count, v); GLDEBUG; }
inline void Graphics::Uniform2f  (int location, float x, float y){ glUniform2f(location, x, y); GLDEBUG; }
inline void Graphics::Uniform2fv (int location, int count, const float* v){ glUniform2fv(location, count, v); GLDEBUG; }
inline void Graphics::Uniform2i  (int location, int x, int y){ glUniform2i(location, x, y); GLDEBUG; }
inline void Graphics::Uniform2iv (int location, int count, const int* v){ glUniform2iv(location, count, v); GLDEBUG; }
inline void Graphics::Uniform3f  (int location, float x, float y, float z){ glUniform3f(location, x, y, z); GLDEBUG; }
inline void Graphics::Uniform3fv (int location, int count, const float* v){ glUniform3fv(location, count, v); GLDEBUG; }
inline void Graphics::Uniform3i  (int location, int x, int y, int z){ glUniform3i(location, x, y, z); GLDEBUG; }
inline void Graphics::Uniform3iv (int location, int count, const int* v){ glUniform3iv(location, count, v); GLDEBUG; }
inline void Graphics::Uniform4f  (int location, float x, float y, float z, float w){ glUniform4f(location, x, y, z, w); GLDEBUG; }
inline void Graphics::Uniform4fv (int location, int count, const float* v){ glUniform4fv(location, count, v); GLDEBUG; }
inline void Graphics::Uniform4i  (int location, int x, int y, int z, int w){ glUniform4i(location, x, y, z, w); GLDEBUG; }
inline void Graphics::Uniform4iv (int location, int count, const int* v){ glUniform4iv(location, count, v); GLDEBUG; }
inline void Graphics::UniformMatrix2fv (int location, int count, unsigned char transpose, const float* value){ glUniformMatrix2fv(location, count, transpose, value); GLDEBUG; }
inline void Graphics::UniformMatrix3fv (int location, int count, unsigned char transpose, const float* value){ glUniformMatrix3fv(location, count, transpose, value); GLDEBUG; }
inline void Graphics::UniformMatrix4fv (int location, int count, unsigned char transpose, const float* value){ glUniformMatrix4fv(location, count, transpose, value); GLDEBUG; }
inline void Graphics::UseProgram (unsigned int program){ glUseProgram(program); GLDEBUG;}
inline void Graphics::ValidateProgram (unsigned int program){ glValidateProgram(program); GLDEBUG; }
inline void Graphics::VertexAttrib1f  (unsigned int indx, float x){ glVertexAttrib1f(indx, x); GLDEBUG; }
inline void Graphics::VertexAttrib1fv (unsigned int indx, const float* values){ glVertexAttrib1fv(indx, values); GLDEBUG; }
inline void Graphics::VertexAttrib2f  (unsigned int indx, float x, float y){ glVertexAttrib2f(indx, x, y); GLDEBUG; }
inline void Graphics::VertexAttrib2fv (unsigned int indx, const float* values){ glVertexAttrib2fv(indx, values); GLDEBUG; }
inline void Graphics::VertexAttrib3f  (unsigned int indx, float x, float y, float z){ glVertexAttrib3f(indx, x, y, z); GLDEBUG; }
inline void Graphics::VertexAttrib3fv (unsigned int indx, const float* values){ glVertexAttrib3fv(indx, values); GLDEBUG; }
inline void Graphics::VertexAttrib4f  (unsigned int indx, float x, float y, float z, float w){ glVertexAttrib4f(indx, x, y, z, w); GLDEBUG; }
inline void Graphics::VertexAttrib4fv (unsigned int indx, const float* values){ glVertexAttrib4fv(indx, values); GLDEBUG; }
 inline void Graphics::VertexAttribPointer (unsigned int indx, int size, VGenum type, unsigned char normalized, int stride, const void* ptr){ glVertexAttribPointer(indx, size, type, normalized, stride, ptr); GLDEBUG; }
inline void Graphics::Viewport (int x, int y, int width, int height){ glViewport(x, y, width, height); GLDEBUG; }

// Extention
/*inline void Graphics::PolygonMode (VGenum face, VGenum mode) {
	glPolygonMode(face, mode); GLDEBUG; 
}*/
	
} // namespace VX
#endif

