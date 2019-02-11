#include <string.h>

namespace
{
	bool serchExtention(const char *extensions, const char *extension)
	{
		if (extensions){
			char *str = (char *)extensions;
			int len = static_cast<int>(strlen(extension));
			
			while ((str = strstr(str, extension)) != NULL) {
				str += len;
				if (*str == ' ' || *str == '\0') {
					return true;
				}
			}
		}
		return false;
	}
	
};// namespace


#if defined(WIN32) || defined(WIN64)
// Win
#include <windows.h>
#include <GL/gl.h>
#include "glext.h"
#include "wglext.h"
	
extern "C"
{
// OpenGL拡張命令の追加方法 (Windowsの場合)
// GLDeviceExtention.cpp に関数ポインタを追加
// InitGLExtention() で関数ポインタを取得
// GLDeviceExtention.h にextern宣言を追加

// ここに必要な拡張命令を記述する
PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB = 0;
PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARB = 0;
PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB = 0;
PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB = 0;
PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB = 0;
PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARB = 0;

PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffers = 0;
PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffers = 0;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebuffer = 0;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2D = 0;
PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffers = 0;
PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffers = 0;
PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbuffer = 0;
PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorage = 0;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbuffer = 0;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatus = 0;

PFNGLGENBUFFERSARBPROC glGenBuffers = 0;                     // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC glBindBuffer = 0;                     // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC glBufferData = 0;                     // VBO Data Loading Procedure
PFNGLBUFFERSUBDATAARBPROC glBufferSubData = 0;               // VBO Sub Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC glDeleteBuffers = 0;               // VBO Deletion Procedure
PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameteriv = 0; // return various parameters of VBO
PFNGLMAPBUFFERARBPROC glMapBuffer = 0;                       // map VBO procedure
PFNGLUNMAPBUFFERARBPROC glUnmapBuffer = 0;                   // unmap VBO procedure
PFNGLISBUFFERARBPROC glIsBuffer = 0;

PFNGLUNIFORM1IPROC glUniform1i = 0;
PFNGLUNIFORM2IPROC glUniform2i = 0;
PFNGLUNIFORM3IPROC glUniform3i = 0;
PFNGLUNIFORM4IPROC glUniform4i = 0;
PFNGLUNIFORM1IVPROC glUniform1iv = 0;
PFNGLUNIFORM2IVPROC glUniform2iv = 0;
PFNGLUNIFORM3IVPROC glUniform3iv = 0;
PFNGLUNIFORM4IVPROC glUniform4iv = 0;
PFNGLUNIFORM1FPROC glUniform1f = 0;
PFNGLUNIFORM2FPROC glUniform2f = 0;
PFNGLUNIFORM3FPROC glUniform3f = 0;
PFNGLUNIFORM4FPROC glUniform4f = 0;
PFNGLUNIFORM1FVPROC glUniform1fv = 0;
PFNGLUNIFORM2FVPROC glUniform2fv = 0;
PFNGLUNIFORM3FVPROC glUniform3fv = 0;
PFNGLUNIFORM4FVPROC glUniform4fv = 0;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = 0;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = 0;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = 0;
PFNGLPROGRAMPARAMETERIEXTPROC glProgramParameteri = 0;

// new
PFNGLACTIVETEXTUREPROC glActiveTexture = 0;
PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture = 0;
PFNGLMULTITEXCOORD1DPROC  glMultiTexCoord1d = 0;
PFNGLMULTITEXCOORD1DVPROC glMultiTexCoord1dv = 0;
PFNGLMULTITEXCOORD1FPROC  glMultiTexCoord1f = 0;
PFNGLMULTITEXCOORD1FVPROC glMultiTexCoord1fv = 0;
PFNGLMULTITEXCOORD1IPROC  glMultiTexCoord1i = 0;
PFNGLMULTITEXCOORD1IVPROC glMultiTexCoord1iv = 0;
PFNGLMULTITEXCOORD1SPROC  glMultiTexCoord1s = 0;
PFNGLMULTITEXCOORD1SVPROC glMultiTexCoord1sv = 0;
PFNGLMULTITEXCOORD2DPROC  glMultiTexCoord2d = 0;
PFNGLMULTITEXCOORD2DVPROC glMultiTexCoord2dv = 0;
PFNGLMULTITEXCOORD2FPROC  glMultiTexCoord2f = 0;
PFNGLMULTITEXCOORD2FVPROC glMultiTexCoord2fv = 0;
PFNGLMULTITEXCOORD2IPROC  glMultiTexCoord2i = 0;
PFNGLMULTITEXCOORD2IVPROC glMultiTexCoord2iv = 0;
PFNGLMULTITEXCOORD2SPROC  glMultiTexCoord2s = 0;
PFNGLMULTITEXCOORD2SVPROC glMultiTexCoord2sv = 0;
PFNGLMULTITEXCOORD3DPROC  glMultiTexCoord3d = 0;
PFNGLMULTITEXCOORD3DVPROC glMultiTexCoord3dv = 0;
PFNGLMULTITEXCOORD3FPROC  glMultiTexCoord3f = 0;
PFNGLMULTITEXCOORD3FVPROC glMultiTexCoord3fv = 0;
PFNGLMULTITEXCOORD3IPROC  glMultiTexCoord3i = 0;
PFNGLMULTITEXCOORD3IVPROC glMultiTexCoord3iv = 0;
PFNGLMULTITEXCOORD3SPROC  glMultiTexCoord3s = 0;
PFNGLMULTITEXCOORD3SVPROC glMultiTexCoord3sv = 0;
PFNGLMULTITEXCOORD4DPROC  glMultiTexCoord4d = 0;
PFNGLMULTITEXCOORD4DVPROC glMultiTexCoord4dv = 0;
PFNGLMULTITEXCOORD4FPROC  glMultiTexCoord4f = 0;
PFNGLMULTITEXCOORD4FVPROC glMultiTexCoord4fv = 0;
PFNGLMULTITEXCOORD4IPROC  glMultiTexCoord4i = 0;
PFNGLMULTITEXCOORD4IVPROC glMultiTexCoord4iv = 0;
PFNGLMULTITEXCOORD4SPROC  glMultiTexCoord4s = 0;
PFNGLMULTITEXCOORD4SVPROC glMultiTexCoord4sv = 0;
PFNGLLOADTRANSPOSEMATRIXFPROC glLoadTransposeMatrixf = 0;
PFNGLLOADTRANSPOSEMATRIXDPROC glLoadTransposeMatrixd = 0;
PFNGLMULTTRANSPOSEMATRIXFPROC glMultTransposeMatrixf = 0;
PFNGLMULTTRANSPOSEMATRIXDPROC glMultTransposeMatrixd = 0;
PFNGLSAMPLECOVERAGEPROC glSampleCoverage = 0;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D = 0;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D = 0;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D = 0;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D = 0;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D = 0;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D = 0;
PFNGLGETCOMPRESSEDTEXIMAGEPROC   glGetCompressedTexImage = 0;

PFNGLBLENDCOLORPROC glBlendColor = 0;
PFNGLBLENDEQUATIONPROC glBlendEquation = 0;

PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate = 0;
PFNGLDRAWBUFFERSPROC glDrawBuffers = 0;
PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate = 0;
PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate = 0;
PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate = 0;
PFNGLATTACHSHADERPROC glAttachShader = 0;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = 0;
PFNGLCOMPILESHADERPROC glCompileShader = 0;
PFNGLCREATEPROGRAMPROC glCreateProgram = 0;
PFNGLCREATESHADERPROC glCreateShader = 0;
PFNGLDELETEPROGRAMPROC glDeleteProgram = 0;
PFNGLDELETESHADERPROC glDeleteShader = 0;
PFNGLDETACHSHADERPROC glDetachShader = 0;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = 0;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = 0;
PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders = 0;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = 0;
PFNGLGETPROGRAMIVPROC glGetProgramiv = 0;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = 0;
PFNGLGETSHADERIVPROC glGetShaderiv = 0;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0;
PFNGLGETSHADERSOURCEPROC glGetShaderSource = 0;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
PFNGLGETUNIFORMFVPROC glGetUniformfv = 0;
PFNGLGETUNIFORMIVPROC glGetUniformiv = 0;
PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv = 0;
PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv = 0;
PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv = 0;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv = 0;
PFNGLISPROGRAMPROC glIsProgram = 0;
PFNGLISSHADERPROC glIsShader = 0;
PFNGLLINKPROGRAMPROC glLinkProgram = 0;
PFNGLSHADERSOURCEPROC glShaderSource = 0;
PFNGLUSEPROGRAMPROC glUseProgram = 0;

PFNGLBLENDFUNCSEPARATEEXTPROC glBlendFuncSeparate = 0;

PFNGLVALIDATEPROGRAMPROC glValidateProgram = 0;
PFNGLVERTEXATTRIB1DPROC  glVertexAttrib1d = 0;
PFNGLVERTEXATTRIB1DVPROC glVertexAttrib1dv = 0;
PFNGLVERTEXATTRIB1FPROC  glVertexAttrib1f = 0;
PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv = 0;
PFNGLVERTEXATTRIB1SPROC  glVertexAttrib1s = 0;
PFNGLVERTEXATTRIB1SVPROC glVertexAttrib1sv = 0;
PFNGLVERTEXATTRIB2DPROC  glVertexAttrib2d = 0;
PFNGLVERTEXATTRIB2DVPROC glVertexAttrib2dv = 0;
PFNGLVERTEXATTRIB2FPROC  glVertexAttrib2f = 0;
PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv = 0;
PFNGLVERTEXATTRIB2SPROC  glVertexAttrib2s = 0;
PFNGLVERTEXATTRIB2SVPROC glVertexAttrib2sv = 0;
PFNGLVERTEXATTRIB3DPROC  glVertexAttrib3d = 0;
PFNGLVERTEXATTRIB3DVPROC glVertexAttrib3dv = 0;
PFNGLVERTEXATTRIB3FPROC  glVertexAttrib3f = 0;
PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv = 0;
PFNGLVERTEXATTRIB3SPROC  glVertexAttrib3s = 0;
PFNGLVERTEXATTRIB3SVPROC glVertexAttrib3sv = 0;
PFNGLVERTEXATTRIB4NBVPROC glVertexAttrib4Nbv = 0;
PFNGLVERTEXATTRIB4NIVPROC glVertexAttrib4Niv = 0;
PFNGLVERTEXATTRIB4NSVPROC glVertexAttrib4Nsv = 0;
PFNGLVERTEXATTRIB4NUBPROC glVertexAttrib4Nub = 0;
PFNGLVERTEXATTRIB4NUBVPROC glVertexAttrib4Nubv = 0;
PFNGLVERTEXATTRIB4NUIVPROC glVertexAttrib4Nuiv = 0;
PFNGLVERTEXATTRIB4NUSVPROC glVertexAttrib4Nusv = 0;
PFNGLVERTEXATTRIB4BVPROC glVertexAttrib4bv = 0;
PFNGLVERTEXATTRIB4DPROC  glVertexAttrib4d = 0; 
PFNGLVERTEXATTRIB4DVPROC glVertexAttrib4dv = 0;  
PFNGLVERTEXATTRIB4FPROC  glVertexAttrib4f = 0; 
PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv = 0;
PFNGLVERTEXATTRIB4IVPROC glVertexAttrib4iv = 0;
PFNGLVERTEXATTRIB4SPROC  glVertexAttrib4s = 0;
PFNGLVERTEXATTRIB4SVPROC glVertexAttrib4sv = 0;
PFNGLVERTEXATTRIB4UBVPROC glVertexAttrib4ubv = 0;
PFNGLVERTEXATTRIB4UIVPROC glVertexAttrib4uiv = 0;
PFNGLVERTEXATTRIB4USVPROC glVertexAttrib4usv = 0;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0;

};

namespace
{
	
bool extensionSupported(const char *extension)
{
	const char* ext = (const char *)::glGetString(GL_EXTENSIONS);
	bool r = serchExtention(ext, extension);

	PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB
		= (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
	const char* wext = 0;
	if (wglGetExtensionsStringARB != 0)
		wext = (const char *)wglGetExtensionsStringARB(::wglGetCurrentDC());

	if (!wext)
	{
		PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT
			= (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
		if (wglGetExtensionsStringEXT != 0)
			wext = (const char *)wglGetExtensionsStringEXT();
	}
	if (wext)
		return r || serchExtention(wext, extension);

	return false;
}

}; // namespace


bool InitGLExtention()
{
	// ここで必要なGL Extention関数ポインタを取得する
	if (!extensionSupported("GL_ARB_multitexture"))
		return false;
	else
	{
		glActiveTexture = (PFNGLACTIVETEXTUREPROC)::wglGetProcAddress("glActiveTexture");
		glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC)::wglGetProcAddress("glClientActiveTexture");
		glMultiTexCoord1d  = (PFNGLMULTITEXCOORD1DPROC)::wglGetProcAddress("glMultiTexCoord1d");
		glMultiTexCoord1dv = (PFNGLMULTITEXCOORD1DVPROC)::wglGetProcAddress("glMultiTexCoord1dv");
		glMultiTexCoord1f  = (PFNGLMULTITEXCOORD1FPROC)::wglGetProcAddress("glMultiTexCoord1f");
		glMultiTexCoord1fv = (PFNGLMULTITEXCOORD1FVPROC)::wglGetProcAddress("glMultiTexCoord1fv");
		glMultiTexCoord1i  = (PFNGLMULTITEXCOORD1IPROC)::wglGetProcAddress("glMultiTexCoord1i");
		glMultiTexCoord1iv = (PFNGLMULTITEXCOORD1IVPROC)::wglGetProcAddress("glMultiTexCoord1iv");
		glMultiTexCoord1s  = (PFNGLMULTITEXCOORD1SPROC)::wglGetProcAddress("glMultiTexCoord1s");
		glMultiTexCoord1sv = (PFNGLMULTITEXCOORD1SVPROC)::wglGetProcAddress("glMultiTexCoord1sv");
		glMultiTexCoord2d  = (PFNGLMULTITEXCOORD2DPROC)::wglGetProcAddress("glMultiTexCoord2d");
		glMultiTexCoord2dv = (PFNGLMULTITEXCOORD2DVPROC)::wglGetProcAddress("glMultiTexCoord2dv");
		glMultiTexCoord2f  = (PFNGLMULTITEXCOORD2FPROC)::wglGetProcAddress("glMultiTexCoord2f");
		glMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FVPROC)::wglGetProcAddress("glMultiTexCoord2fv");
		glMultiTexCoord2i  = (PFNGLMULTITEXCOORD2IPROC)::wglGetProcAddress("glMultiTexCoord2i");
		glMultiTexCoord2iv = (PFNGLMULTITEXCOORD2IVPROC)::wglGetProcAddress("glMultiTexCoord2iv");
		glMultiTexCoord2s  = (PFNGLMULTITEXCOORD2SPROC)::wglGetProcAddress("glMultiTexCoord2s");
		glMultiTexCoord2sv = (PFNGLMULTITEXCOORD2SVPROC)::wglGetProcAddress("glMultiTexCoord2sv");
		glMultiTexCoord3d  = (PFNGLMULTITEXCOORD3DPROC)::wglGetProcAddress("glMultiTexCoord3d");
		glMultiTexCoord3dv = (PFNGLMULTITEXCOORD3DVPROC)::wglGetProcAddress("glMultiTexCoord3dv");
		glMultiTexCoord3f  = (PFNGLMULTITEXCOORD3FPROC)::wglGetProcAddress("glMultiTexCoord3f");
		glMultiTexCoord3fv = (PFNGLMULTITEXCOORD3FVPROC)::wglGetProcAddress("glMultiTexCoord3fv");
		glMultiTexCoord3i  = (PFNGLMULTITEXCOORD3IPROC)::wglGetProcAddress("glMultiTexCoord3i");
		glMultiTexCoord3iv = (PFNGLMULTITEXCOORD3IVPROC)::wglGetProcAddress("glMultiTexCoord3iv");
		glMultiTexCoord3s  = (PFNGLMULTITEXCOORD3SPROC)::wglGetProcAddress("glMultiTexCoord3s");
		glMultiTexCoord3sv = (PFNGLMULTITEXCOORD3SVPROC)::wglGetProcAddress("glMultiTexCoord3sv");
		glMultiTexCoord4d  = (PFNGLMULTITEXCOORD4DPROC)::wglGetProcAddress("glMultiTexCoord4d");
		glMultiTexCoord4dv = (PFNGLMULTITEXCOORD4DVPROC)::wglGetProcAddress("glMultiTexCoord4dv");
		glMultiTexCoord4f  = (PFNGLMULTITEXCOORD4FPROC)::wglGetProcAddress("glMultiTexCoord4f");
		glMultiTexCoord4fv = (PFNGLMULTITEXCOORD4FVPROC)::wglGetProcAddress("glMultiTexCoord4fv");
		glMultiTexCoord4i  = (PFNGLMULTITEXCOORD4IPROC)::wglGetProcAddress("glMultiTexCoord4i");
		glMultiTexCoord4iv = (PFNGLMULTITEXCOORD4IVPROC)::wglGetProcAddress("glMultiTexCoord4iv");
		glMultiTexCoord4s  = (PFNGLMULTITEXCOORD4SPROC)::wglGetProcAddress("glMultiTexCoord4s");
		glMultiTexCoord4sv = (PFNGLMULTITEXCOORD4SVPROC)::wglGetProcAddress("glMultiTexCoord4sv");
		glLoadTransposeMatrixf = (PFNGLLOADTRANSPOSEMATRIXFPROC)::wglGetProcAddress("glLoadTransposeMatrixf");
		glLoadTransposeMatrixd = (PFNGLLOADTRANSPOSEMATRIXDPROC)::wglGetProcAddress("glLoadTransposeMatrixd");
		glMultTransposeMatrixf = (PFNGLMULTTRANSPOSEMATRIXFPROC)::wglGetProcAddress("glMultTransposeMatrixf");
		glMultTransposeMatrixd = (PFNGLMULTTRANSPOSEMATRIXDPROC)::wglGetProcAddress("glMultTransposeMatrixd");
		glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC)::wglGetProcAddress("glSampleCoverage");
		glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC)::wglGetProcAddress("glCompressedTexImage3D");
		glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)::wglGetProcAddress("glCompressedTexImage2D");
		glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC)::wglGetProcAddress("glCompressedTexImage1D");
		glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)::wglGetProcAddress("glCompressedTexSubImage3D");
		glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)::wglGetProcAddress("glCompressedTexSubImage2D");
		glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)::wglGetProcAddress("glCompressedTexSubImage1D");
		glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC)::wglGetProcAddress("glGetCompressedTexImage");
	}

	if (!extensionSupported("GL_ARB_texture_non_power_of_two"))
		return false;

	if (!extensionSupported("WGL_ARB_pbuffer"))
		return false;
	else
	{
		wglCreatePbufferARB = (PFNWGLCREATEPBUFFERARBPROC)::wglGetProcAddress("wglCreatePbufferARB");
		wglGetPbufferDCARB = (PFNWGLGETPBUFFERDCARBPROC)::wglGetProcAddress("wglGetPbufferDCARB");
		wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC)::wglGetProcAddress("wglReleasePbufferDCARB");
		wglDestroyPbufferARB = (PFNWGLDESTROYPBUFFERARBPROC)::wglGetProcAddress("wglDestroyPbufferARB");
		wglQueryPbufferARB = (PFNWGLQUERYPBUFFERARBPROC)::wglGetProcAddress("wglQueryPbufferARB");
		wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)::wglGetProcAddress("wglGetPixelFormatAttribivARB");
	}
	
	if (!extensionSupported("GL_EXT_framebuffer_object"))
		return false;
	else
	{
		glGenFramebuffers = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
		glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
		glBindFramebuffer = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
		glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
		glGenRenderbuffers = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
		glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
		glBindRenderbuffer = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
		glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
		glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
		glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
	}

	// check PBO is supported by your video card
    if (!extensionSupported("GL_ARB_pixel_buffer_object"))
		return false;
	else
	{
		// get pointers to GL functions
		glGenBuffers = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
		glBindBuffer = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
		glBufferData = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
		glBufferSubData = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
		glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
		glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
		glMapBuffer = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
		glUnmapBuffer = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");
		glIsBuffer = (PFNGLISBUFFERARBPROC)wglGetProcAddress("glIsBufferARB");
    }

	if (!extensionSupported("GL_ARB_shader_objects"))
		return false;
	else
	{
		glGetUniformfv = (PFNGLGETUNIFORMFVPROC)wglGetProcAddress("glGetUniformfv");
		glGetUniformiv = (PFNGLGETUNIFORMIVPROC )wglGetProcAddress("glGetUniformiv");
		glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC)wglGetProcAddress("glGetVertexAttribdv");
		glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)wglGetProcAddress("glGetVertexAttribfv");
		glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)wglGetProcAddress("glGetVertexAttribiv");
		glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)wglGetProcAddress("glGetVertexAttribPointerv");
		glIsProgram = (PFNGLISPROGRAMPROC)wglGetProcAddress("glIsProgram");
		glIsShader = (PFNGLISSHADERPROC)wglGetProcAddress("glIsShader");

		glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
		glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
		glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
		glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
		glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
		glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
		glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
		glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC)wglGetProcAddress("glGetAttachedShaders");
		glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
		glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
		glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
		glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
		glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
		glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
		glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
		// get vertex attribute function pointers
		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
		glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
		glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)wglGetProcAddress("glGetActiveAttrib");
		glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");
		glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
		// uniforms
		glGetShaderSource = (PFNGLGETSHADERSOURCEPROC)wglGetProcAddress("glGetShaderSource");
		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
		glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
		glUniform2i = (PFNGLUNIFORM2IPROC)wglGetProcAddress("glUniform2i");
		glUniform3i = (PFNGLUNIFORM3IPROC)wglGetProcAddress("glUniform3i");
		glUniform4i = (PFNGLUNIFORM4IPROC)wglGetProcAddress("glUniform4i");
		glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
		glUniform2iv = (PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");
		glUniform3iv = (PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");
		glUniform4iv = (PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");
		glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
		glUniform2f = (PFNGLUNIFORM2FPROC)wglGetProcAddress("glUniform2f");
		glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
		glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
		glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
		glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
		glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
		glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
		glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)wglGetProcAddress("glUniformMatrix2fv");
		glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress("glUniformMatrix3fv");
		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
		glProgramParameteri = (PFNGLPROGRAMPARAMETERIEXTPROC)wglGetProcAddress("glProgramParameteri");

		glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)wglGetProcAddress("glValidateProgram");
		glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC)wglGetProcAddress("glVertexAttrib1d");
		glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC)wglGetProcAddress("glVertexAttrib1dv");
		glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)wglGetProcAddress("glVertexAttrib1f");
		glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)wglGetProcAddress("glVertexAttrib1fv");
		glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC)wglGetProcAddress("glVertexAttrib1s");
		glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC)wglGetProcAddress("glVertexAttrib1sv");
		glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC)wglGetProcAddress("glVertexAttrib2d");
		glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC)wglGetProcAddress("glVertexAttrib2dv");
		glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC)wglGetProcAddress("glVertexAttrib2f");
		glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)wglGetProcAddress("glVertexAttrib2fv");
		glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC)wglGetProcAddress("glVertexAttrib2s");
		glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC)wglGetProcAddress("glVertexAttrib2sv");
		glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC)wglGetProcAddress("glVertexAttrib3d");
		glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC)wglGetProcAddress("glVertexAttrib3dv");
		glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC)wglGetProcAddress("glVertexAttrib3f");
		glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)wglGetProcAddress("glVertexAttrib3fv");
		glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC)wglGetProcAddress("glVertexAttrib3s");
		glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC)wglGetProcAddress("glVertexAttrib3sv");
		glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC)wglGetProcAddress("glVertexAttrib4Nbv");
		glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC)wglGetProcAddress("glVertexAttrib4Niv");
		glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC)wglGetProcAddress("glVertexAttrib4Nsv");
		glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC)wglGetProcAddress("glVertexAttrib4Nub");
		glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC)wglGetProcAddress("glVertexAttrib4Nubv");
		glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC)wglGetProcAddress("glVertexAttrib4Nuiv");
		glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC)wglGetProcAddress("glVertexAttrib4Nusv");
		glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC )wglGetProcAddress("glVertexAttrib4bv");
		glVertexAttrib4d  = (PFNGLVERTEXATTRIB4DPROC  )wglGetProcAddress("glVertexAttrib4d");
		glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC )wglGetProcAddress("glVertexAttrib4dv");
		glVertexAttrib4f  = (PFNGLVERTEXATTRIB4FPROC  )wglGetProcAddress("glVertexAttrib4f");
		glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC )wglGetProcAddress("glVertexAttrib4fv");
		glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC )wglGetProcAddress("glVertexAttrib4iv");
		glVertexAttrib4s  = (PFNGLVERTEXATTRIB4SPROC  )wglGetProcAddress("glVertexAttrib4s");
		glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC )wglGetProcAddress("glVertexAttrib4sv");
		glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC)wglGetProcAddress("glVertexAttrib4ubv");
		glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC)wglGetProcAddress("glVertexAttrib4uiv");
		glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC)wglGetProcAddress("glVertexAttrib4usv");
		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	}

	if (!extensionSupported("GL_EXT_blend_func_separate"))
		return false;
	else
	{
		glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEEXTPROC)wglGetProcAddress("glBlendFuncSeparateEXT");
	}

	if (!extensionSupported("GL_EXT_blend_color"))
		return false;
	else
		glBlendColor = (PFNGLBLENDCOLORPROC)wglGetProcAddress("glBlendColor");
	
	if (!extensionSupported("GL_EXT_blend_minmax"))
		return false;
	else
		glBlendEquation = (PFNGLBLENDEQUATIONPROC)wglGetProcAddress("glBlendEquation");

	if (!extensionSupported("GL_EXT_blend_equation_separate"))
		return false;
	else
		glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)wglGetProcAddress("glBlendEquationSeparate");

	// GL_VERSION_2_0
	glDrawBuffers = (PFNGLDRAWBUFFERSPROC)wglGetProcAddress("glDrawBuffers");
	glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)wglGetProcAddress("glStencilOpSeparate");
	glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)wglGetProcAddress("glStencilFuncSeparate");
	glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC)wglGetProcAddress("glStencilMaskSeparate");

	return true;
}

#else

#if defined(__APPLE__) || defined(MACOSX)
// MacOSX
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
// Linux
#ifdef USE_OSMesa
#include <GL/osmesa.h>
#else
#include <GL/gl.h>
#endif
#include <GL/glext.h>
#endif
namespace
{

bool extensionSupported(const char *extension)
{
	const char* ext = (const char *)glGetString(GL_EXTENSIONS);
	bool r = serchExtention(ext, extension);
	
	return r;
}
	
}; // namespace

bool InitGLExtention()
{
	if (!extensionSupported("GL_ARB_multitexture"))
		return false;
	if (!extensionSupported("GL_ARB_texture_non_power_of_two"))
		return false;
	if (!extensionSupported("GL_EXT_framebuffer_object"))
		return false;
    if (!extensionSupported("GL_ARB_pixel_buffer_object"))
		return false;
	if (!extensionSupported("GL_ARB_shader_objects"))
		return false;
	
	return true;
}

#endif
