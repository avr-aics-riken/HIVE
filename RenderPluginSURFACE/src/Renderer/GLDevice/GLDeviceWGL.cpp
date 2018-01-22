#if defined(_WIN32) || defined(_WIN64)

#include "GLDeviceWGL.h"
#include <windows.h>
#include <stdio.h>

#include "GLDeviceExtention.h"


bool GLDeviceWGL::Init(int width, int height, int colorbit, int depthbit, bool use_pbuffer)
{
	PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
		1,                     // version number 
		PFD_DRAW_TO_WINDOW |   // WINDOW を指定すること
		PFD_SUPPORT_OPENGL |
		//PFD_DOUBLEBUFFER,    // support OpenGL 
		PFD_TYPE_RGBA,         // RGBA type 
		colorbit,              // 24-bit color depth 
		0, 0, 0, 0, 0, 0,      // color bits ignored 
		0,                     // no alpha buffer 
		0,                     // shift bit ignored 
		0,                     // no accumulation buffer 
		0, 0, 0, 0,            // accum bits ignored 
		depthbit,              // 32-bit z-buffer     
		0,                     // no stencil buffer 
		0,                     // no auxiliary buffer 
		PFD_MAIN_PLANE,        // main layer 
		0,                     // reserved 
		0, 0, 0                // layer masks ignored 
	}; 

	// DummyWindow不可視
	int style = WS_POPUPWINDOW | (use_pbuffer ? 0 : WS_VISIBLE);
	int win_wid = (use_pbuffer ? 128 : width);
	int win_hei = (use_pbuffer ? 128 : height);

	HWND dummy_hWnd = CreateWindowA("STATIC","", style, 0,0,win_wid,win_hei, NULL,
		NULL, (HINSTANCE)GetModuleHandleA(NULL), NULL);
	HDC dummy_hDC = GetDC(dummy_hWnd);

	
	int	iPixelFormat = ::ChoosePixelFormat(dummy_hDC, &pfd);
	BOOL success = ::SetPixelFormat(dummy_hDC, iPixelFormat, &pfd);

	HGLRC dummy_hRC = ::wglCreateContext(dummy_hDC);
	::wglMakeCurrent(dummy_hDC, dummy_hRC);// ダミーをセット(for wgl)
	
	if (initWGL() == false)
	{
		::wglMakeCurrent(NULL, NULL);
		return false;
	}

	if (use_pbuffer == false)
	{
		m_hWnd = dummy_hWnd;
		m_hDC  = dummy_hDC;
		m_hRC  = dummy_hRC;
	}
	else
	{
		// pbufferの最大サイズを取得
		int iAttributes[] = {
			WGL_MAX_PBUFFER_WIDTH_ARB, WGL_MAX_PBUFFER_HEIGHT_ARB,
			WGL_MAX_PBUFFER_PIXELS_ARB
		};
		int iValues[3];
		if (wglGetPixelFormatAttribivARB)
			::wglGetPixelFormatAttribivARB(::wglGetCurrentDC(), ::GetPixelFormat(::wglGetCurrentDC()), 0, 3, iAttributes, iValues);
		if (iValues[0] < width || iValues[1] < height)
		{
			return false;
		}
		m_pbuffer_width = width;
		m_pbuffer_height = height;

		// pbuffer 作成
		int iAttribList[] = {0};
		m_hPbufferARB = ::wglCreatePbufferARB(dummy_hDC, iPixelFormat, m_pbuffer_width, m_pbuffer_height, iAttribList);
		if (!m_hPbufferARB)
		{
			return false;
		}
		::wglQueryPbufferARB(m_hPbufferARB, WGL_PBUFFER_WIDTH_ARB, &m_pbuffer_width);
		::wglQueryPbufferARB(m_hPbufferARB, WGL_PBUFFER_HEIGHT_ARB, &m_pbuffer_height);

		// pbufferからHDCとGLコンテキストを作成
		m_hDC = ::wglGetPbufferDCARB(m_hPbufferARB);
		iPixelFormat = ::ChoosePixelFormat(m_hDC, &pfd);
		success = ::SetPixelFormat(m_hDC, iPixelFormat, &pfd);
		m_hRC = ::wglCreateContext(m_hDC);

		// pbufferをバインド
		::wglMakeCurrent(m_hDC, m_hRC);

		// dummyを削除
		wglDeleteContext(dummy_hRC);
		ReleaseDC(dummy_hWnd, dummy_hDC);
		DestroyWindow(dummy_hWnd);
	}

	return true;
}

void GLDeviceWGL::Deinit()
{
	if (m_hPbufferARB)
	{
		if (m_hDC)
		{
			wglReleasePbufferDCARB(m_hPbufferARB, m_hDC);
			m_hDC = NULL;
		}
		wglDestroyPbufferARB(m_hPbufferARB);
		m_hPbufferARB = NULL;
	}

	if (m_hRC)
	{
		::wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}

	if (m_hDC)
	{
		ReleaseDC(m_hWnd, m_hDC);
		m_hDC = NULL;
	}
	if (m_hWnd)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}
}
	
void GLDeviceWGL::SwapBuffers()
{
	::SwapBuffers(m_hDC);
}

bool GLDeviceWGL::initWGL()
{
	const char* version;
	int majorVersion, minorVersion;
	
	version = (const char*)::glGetString(GL_VERSION);
	sscanf(version, "%1d.%1d", &majorVersion, &minorVersion);

	if (!InitGLExtention())
		return false;

	return true;
}

void GLDeviceWGL::SetCurrent()
{
	wglMakeCurrent(m_hDC, m_hRC);
}

void GLDeviceWGL::UnsetCurrent()
{
	wglMakeCurrent(NULL, NULL);
}

#endif // Windows
