#if defined(__APPLE__) || defined(MACOSX)

// MacOSX

#include "GLDeviceCGL.h"
#include "GLDeviceExtention.h"
#include <stdio.h>
#include <stdlib.h>


namespace
{
//	bool make_window(const char* name, int colorbit, int depthbit, int width, int height, CGLWindowObj *winRet, CGLContextObj *ctxRet)
//	{
//		return false;
//	}
	
	bool make_pbuffer(int colorbit, int depthbit, int width, int height, CGLPBufferObj* pbuffer, CGLContextObj *ctxRet)
	{
		int err=0;
		CGLPixelFormatObj pixelFormat;
		CGLContextObj oldContext = CGLGetCurrentContext();
		CGLPBufferObj pbuf;
		CGLContextObj ctx;
		
		CGLPixelFormatAttribute attribs[]={
			kCGLPFAWindow,
			kCGLPFAAlphaSize,(CGLPixelFormatAttribute)8,
			kCGLPFAColorSize,(CGLPixelFormatAttribute)colorbit,
			kCGLPFADepthSize,(CGLPixelFormatAttribute)depthbit,
			kCGLPFAAccelerated,
			kCGLPFAOpenGLProfile, (CGLPixelFormatAttribute)kCGLOGLPVersion_Legacy,
			(CGLPixelFormatAttribute)0
		};
		
        GLint numberOfVirtual;
		err = CGLChoosePixelFormat(attribs, &pixelFormat, &numberOfVirtual);
		if(err)
		{
			printf("Fail: CGLChoosePixelFormat\n");
			return false;
		}
		
		err = CGLCreateContext(pixelFormat, oldContext, &ctx);
		if(err)
		{
			printf("Fail: CGLCreateContext\n");
			return false;
		}
		
		err = CGLCreatePBuffer(width, height, GL_TEXTURE_2D, GL_RGBA, 0, &pbuf);
		if(err)
		{
			printf("Fail: CGLCreatePBuffer\n");
			return false;
		}
		
		CGLDestroyPixelFormat(pixelFormat);
		
		*pbuffer = pbuf;
		*ctxRet  = ctx;
		return true;
	}
	
};// namespace

GLDeviceCGL::GLDeviceCGL()
{
	m_pbuffer = 0;
	m_use_pbuffer = true;
	m_ctx = 0;
	m_width = 0;
	m_height = 0;
}

GLDeviceCGL::~GLDeviceCGL()
{
	Deinit();
}


bool GLDeviceCGL::Init(int width, int height, int colorbit, int depthbit, bool use_pbuffer)
{
	m_use_pbuffer = use_pbuffer;
	m_width = width;
	m_height = height;

	if (m_use_pbuffer)
	{
		bool r = make_pbuffer(colorbit, depthbit, m_width, m_height, &m_pbuffer, &m_ctx);
		if (!r)
		{
			printf("Fail: make_pbuffer\n");
			return false;
		}
		
		GLint vs;
		long err;
		err = CGLGetVirtualScreen(m_ctx, &vs);
		if (err)
		{
			printf("Fail: CGLGetVirtualScreen\n");
			return false;
		}
		
		err = CGLSetCurrentContext(m_ctx);
		if (err)
		{
			printf("Fail: CGLSetCurrentContext\n");
			return false;
		}
		
		err=CGLSetPBuffer(m_ctx, m_pbuffer, 0, 0, vs);
		if (err)
		{
			printf("Fail: CGLSetPBuffer\n");
			return false;
		}
		
	}
	else
	{
		//bool r = make_window("GL ContextWindow", colorbit, depthbit, m_width, m_height, &m_win, &m_ctx);
		//if (!r)
		//{
		//	return false;
		//}
		// TODO
		//XMapWindow(m_display, m_win);
		//glXMakeCurrent(m_display, m_win, m_ctx);
	}
	
	if (InitGLExtention() == false)
	{
		printf("Error: not supported GL Extentions\n");
		return false;
	}
	return true;
}
	
void GLDeviceCGL::Deinit()
{
	// GL Context
	if (m_ctx)
	{
		CGLDestroyContext(m_ctx);
		m_ctx = 0;
	}
	
	// pbuffer or window
	if (m_use_pbuffer)
	{
		if (m_pbuffer)
		{
			CGLDestroyPBuffer(m_pbuffer);
			m_pbuffer = 0;
		}
	}
	else
	{
		//if (m_win)
		//{
			//XDestroyWindow(m_display, m_win); // TODO
		//	m_win = 0;
		//}
	}
	
}

bool GLDeviceCGL::Resize(int width,int height)
{
  m_width = width;
  m_height = height;
  int err = CGLSetCurrentContext(m_ctx);
	if (m_ctx){
		if (m_pbuffer){
			CGLDestroyPBuffer(m_pbuffer);
			m_pbuffer = 0;
		}
		int err;
		err = CGLCreatePBuffer(width, height, GL_TEXTURE_2D, GL_RGBA, 0, &m_pbuffer);
		if(err){
			printf("Fail: CGLCreatePBuffer\n");
			return false;
		}
		GLint vs;
		err = CGLGetVirtualScreen(m_ctx, &vs);
		if (err){
			printf("Fail: CGLGetVirtualScreen\n");
			return false;
		}
		err = CGLSetPBuffer(m_ctx, m_pbuffer, 0, 0, vs);
		if (err)
		{
			printf("Fail: CGLSetPBuffer\n");
			return false;
		}
		glViewport(0, 0, width, height);
		SetCurrent();
		return true;
	}
	return false;
}

void GLDeviceCGL::SwapBuffers()
{
	if (m_ctx)
		CGLFlushDrawable(m_ctx);
}

void GLDeviceCGL::SetCurrent()
{
	CGLSetCurrentContext(m_ctx);
}

void GLDeviceCGL::UnsetCurrent()
{
	CGLSetCurrentContext(NULL);
}

GLDevice* CreateGLDeviceInstance()
{
	return new GLDeviceCGL();
}

	
#endif // MacOSX
