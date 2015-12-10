#include <string.h>
#include <stdio.h>

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
#include <GL/glext.h>
#include <GL/wglext.h>

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

PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT = 0;
PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT = 0;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT = 0;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT = 0;
PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT = 0;
PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT = 0;
PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT = 0;
PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT = 0;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT = 0;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT = 0;

PFNGLACTIVETEXTUREPROC glActiveTexture = 0;

PFNGLGENBUFFERSARBPROC glGenBuffersARB = 0;                     // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC glBindBufferARB = 0;                     // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC glBufferDataARB = 0;                     // VBO Data Loading Procedure
PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB = 0;               // VBO Sub Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = 0;               // VBO Deletion Procedure
PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB = 0; // return various parameters of VBO
PFNGLMAPBUFFERARBPROC glMapBufferARB = 0;                       // map VBO procedure
PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB = 0;                   // unmap VBO procedure

PFNGLCREATESHADERPROC glCreateShader = 0;
PFNGLSHADERSOURCEPROC glShaderSource = 0;
PFNGLCOMPILESHADERPROC glCompileShader = 0;
PFNGLGETSHADERIVPROC glGetShaderiv = 0;
PFNGLDELETESHADERPROC glDeleteShader = 0;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0;
PFNGLCREATEPROGRAMPROC glCreateProgram = 0;
PFNGLATTACHSHADERPROC glAttachShader = 0;
PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders = 0;
PFNGLLINKPROGRAMPROC glLinkProgram = 0;
PFNGLGETPROGRAMIVPROC glGetProgramiv = 0;
PFNGLUSEPROGRAMPROC glUseProgram = 0;
PFNGLDETACHSHADERPROC glDetachShader = 0;
PFNGLDELETEPROGRAMPROC glDeleteProgram = 0;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = 0;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = 0;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = 0;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = 0;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
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
	if (!extensionSupported("GL_ARB_multitexture")) {
		printf("Not found GL_ARB_multitexture\n");
		return false;
	}
	else
	{
		glActiveTexture = (PFNGLACTIVETEXTUREPROC)::wglGetProcAddress("glActiveTexture");
	}

	if (!extensionSupported("GL_ARB_texture_non_power_of_two")) {
		printf("Not found GL_ARB_texture_non_power_of_two\n");
		return false;
	}

	if (!extensionSupported("WGL_ARB_pbuffer")) {
		printf("Not found WGL_ARB_pbuffer\n");
		return false;
	}
	else
	{
		wglCreatePbufferARB = (PFNWGLCREATEPBUFFERARBPROC)::wglGetProcAddress("wglCreatePbufferARB");
		wglGetPbufferDCARB = (PFNWGLGETPBUFFERDCARBPROC)::wglGetProcAddress("wglGetPbufferDCARB");
		wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC)::wglGetProcAddress("wglReleasePbufferDCARB");
		wglDestroyPbufferARB = (PFNWGLDESTROYPBUFFERARBPROC)::wglGetProcAddress("wglDestroyPbufferARB");
		wglQueryPbufferARB = (PFNWGLQUERYPBUFFERARBPROC)::wglGetProcAddress("wglQueryPbufferARB");
		wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)::wglGetProcAddress("wglGetPixelFormatAttribivARB");
	}

	if (!extensionSupported("GL_EXT_framebuffer_object")) {
		printf("Not found GL_EXT_framebuffer_object\n");
		return false;
	}
	else
	{
		glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
		glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
		glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
		glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
		glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
		glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
		glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
		glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
		glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
		glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
	}

	// check PBO is supported by your video card
    if (!extensionSupported("GL_ARB_pixel_buffer_object")) {
		printf("Not found GL_ARB_pixel_buffer_object");
		return false;
	}
	else
	{
		// get pointers to GL functions
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
		glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
		glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
		glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
		glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
		glMapBufferARB = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
		glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");
    }

	if (!extensionSupported("GL_ARB_shader_objects")) {
		printf("Not found GL_ARB_shader_objects");
		return false;
	}
	else
	{
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
		glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
		// uniforms
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
	}
	return true;
}

#else

#if defined(__APPLE__) || defined(MACOSX)
// MacOSX
	#include <OpenGL/OpenGL.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	namespace
	{
		bool extensionSupported(const char *extension)
		{
			const char* ext = (const char *)glGetString(GL_EXTENSIONS);
			bool r = serchExtention(ext, extension);
			return r;
		}
	} //namespace
#else
// Linux
	#ifdef USE_OSMesa
	#include <GL/osmesa.h>
	#else
	#include <GL/gl.h>
	#endif
	#define GL_GLEXT_PROTOTYPES
	#include <GL/glext.h>
	#include <GL/glx.h>
	namespace
	{
		PFNGLGETSTRINGIPROC glGetStringi = 0;
		inline bool extensionSupported(const char *extension)
		{
			glGetStringi = (PFNGLGETSTRINGIPROC)glXGetProcAddress((const GLubyte*)"glGetStringi");
			bool r = false;
			GLint n = 30000; // because glGetIntegerv(GL_NUM_EXTENSIONS, &n); returns bad value by LSGL
			for (GLint i = 0; i < n; ++i) {
				const char* exti = (const char*)glGetStringi(GL_EXTENSIONS, (GLuint)i);
				if (serchExtention(exti, extension)) {
					r = true;
					break;
				}
			}
			return r;
		}
	} //namespace
#endif

bool InitGLExtention()
{
	if (!extensionSupported("GL_ARB_multitexture")) {
		printf("Not found GL_ARB_multitexture\n");
		return false;
	}
	if (!extensionSupported("GL_ARB_texture_non_power_of_two")) {
		printf("Not found GL_ARB_texture_non_power_of_two\n");
		return false;
	}
	if (!extensionSupported("GL_EXT_framebuffer_object")) {
		printf("Not found GL_EXT_framebuffer_object\n");
		return false;
	}
	if (!extensionSupported("GL_ARB_pixel_buffer_object")) {
		printf("Not found GL_ARB_pixel_buffer_object\n");
		return false;
	}
	if (!extensionSupported("GL_ARB_shader_objects")) {
		printf("Not found GL_ARB_shader_objects\n");
		return false;
	}

	return true;
}

#endif
