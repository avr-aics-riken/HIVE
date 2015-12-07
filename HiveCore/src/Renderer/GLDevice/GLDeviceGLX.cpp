#if defined(_WIN32) || defined(_WIN64)
// win

#else
// linux

#include "GLDeviceGLX.h"
#include "GLDeviceExtention.h"
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>


namespace
{
	bool make_window( Display *dpy, const char *name, int colorbit, int depthbit,
					 int width, int height, Window *winRet, GLXContext *ctxRet)
	{
		int attrib[] = {
		        GLX_RGBA, GLX_DOUBLEBUFFER,
			GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8,
			GLX_DEPTH_SIZE, depthbit,
			None
		};
		int scrnum;
		XSetWindowAttributes attr;
		unsigned long mask;
		Window root;
		Window win;
		GLXContext ctx;
		XVisualInfo *visinfo;
	
		scrnum = DefaultScreen(dpy);
		//scrnum = 0;
		root = RootWindow(dpy, scrnum);
	
		visinfo = glXChooseVisual( dpy, scrnum, attrib );
		if (!visinfo) {
			printf("Error: couldn't get an RGB, Double-buffered visual\n");
			return false;
		}
		
		/* window attributes */
		attr.background_pixel = 0;
		attr.border_pixel = 0;
		attr.colormap = XCreateColormap( dpy, root, visinfo->visual, AllocNone);
		attr.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask;
		mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;
		
		win = XCreateWindow( dpy, root, 0, 0, width, height,
							0, visinfo->depth, InputOutput,
							visinfo->visual, mask, &attr );
		if (!win)
		{
			printf("Error: couldn't create window.");
			return false;
		}
		
		/* set hints and properties */
		XSizeHints sizehints;
		sizehints.x = 0;
		sizehints.y = 0;
		sizehints.width  = width;
		sizehints.height = height;
		sizehints.flags = USSize | USPosition;
		XSetNormalHints(dpy, win, &sizehints);
		XSetStandardProperties(dpy, win, name, name,
							   None, (char **)NULL, 0, &sizehints);
				
		ctx = glXCreateContext( dpy, visinfo, NULL, True );
		if (!ctx) {
			printf("Error: glXCreateContext failed\n");
			return false;
		}
		
		XFree(visinfo);
		*winRet = win;
		*ctxRet = ctx;
		return true;
	}
	
	bool make_pbuffer(Display* dpy, int colorbit, int depthbit, int width, int height, GLXPbuffer* pbuffer, GLXContext *ctxRet)
	{
	    int attrib[] = { 
			GLX_RENDER_TYPE, GLX_RGBA_BIT, //GLX_DOUBLEBUFFER, 
			GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8,
			GLX_DEPTH_SIZE, depthbit,
			None 
	    }; 
		
		int scrnum = DefaultScreen( dpy );
		printf("Info: DefaultScreen=%d\n", scrnum);
		GLXFBConfig* configs;
		int nconfigs = 0;
		configs = glXChooseFBConfig(dpy, scrnum, attrib, &nconfigs);
		printf("Info: glXChooseFBConfig=%d\n", nconfigs);
		
		int pbattrs[] = {
			GLX_PBUFFER_WIDTH, width,
			GLX_PBUFFER_HEIGHT, height,
			0
		};
		GLXPbuffer pbuf = glXCreatePbuffer(dpy, *configs, pbattrs);
		if (!pbuf)
		{
			printf("Error: glxCreatePbuffer\n");
			return false;
		}
		printf("Info: glXCreatePbuffer\n");
		
		GLXContext ctx = glXCreateNewContext(dpy, *configs, GLX_RGBA_TYPE, 0, True);
		if (!ctx)
		{
			printf("Error: glXCreateNewContext\n");
			return false;
		}
		printf("Info: glXCreateNewContext\n");
		
		*pbuffer = pbuf;
		*ctxRet  = ctx;
		return true;
	}
	
	bool checkFile(const char* filename)
	{
		// TODO: エラーチェック
		return true;

		//FILE* fp = fopen(filename, "r");
		//if (!fp)
		//	return false;
		//else
		//	fclose(fp);
		//	
		//struct stat sb;
		//if (stat(filename, &sb) == -1)
		//{
		//	perror(filename);
		//	return false;
		//}
		////can exec?
		//int canExe = S_IXUSR;
		//if (sb.st_mode & canExe)
		//	return true;
		//
		//return false;
	}
	
	bool checkScripts()
	{
		bool r = checkFile("startx.sh");
		r &= checkFile("sleep.sh");		
		r &= checkFile("killx.sh");
		return r;
	}
	
	bool launchedX = false;
	Display* StartX(const char* dispStr)
	{
		Display* disp = XOpenDisplay(dispStr);
		if (disp)
		{
			return disp;
		}
		else
		{
			// Script check
			if (!checkScripts())
			{
				printf("Scripts are not find or don't have permissions of execution.\n");
				return 0;
			}
			
			launchedX = true;
			printf("Launch XWindow\n");
			system("startx.sh");
			unsigned int cnt = 0;
			while(!disp)
			{
				disp = XOpenDisplay(dispStr);
				sleep(1);
				cnt++;
				if (cnt > 30)
				{
					launchedX = false;
					printf("Error: Timeout watinng x startup.\n");
					break;
				}
			}
			return disp;
		}
	}
	void KillX()
	{
		if (launchedX)
		{
			printf("KILL XWindow\n");
			launchedX = false;
			system("killx.sh");
		}
	}
};// namespace
				 
				 
bool GLDeviceGLX::Init(int width, int height, int colorbit, int depthbit, bool use_pbuffer)
{
	m_use_pbuffer = use_pbuffer;
	m_width = width;
	m_height = height;

	m_display = StartX(":0.0");
	
	if (!m_display) {
		printf("Error: couldn't open display %s\n", m_display);
		return false;
	}
		
	if (m_use_pbuffer)
	{
		bool r = make_pbuffer(m_display, colorbit, depthbit, m_width, m_height, &m_pbuffer, &m_ctx);
		if (!r)
		{
			KillX();
			return false;
		}
		glXMakeCurrent(m_display, m_pbuffer, m_ctx);
	}
	else
	{
		bool r = make_window(m_display, "GL ContextWindow", colorbit, depthbit, m_width, m_height, &m_win, &m_ctx);
		if (!r)
		{
			KillX();
			return false;
		}
		XMapWindow(m_display, m_win);
		glXMakeCurrent(m_display, m_win, m_ctx);
	}
	
	if (InitGLExtention() == false)
	{
		printf("Error: not supported GL Extentions＼n");
		return false;
	}
	return true;
}
	
void GLDeviceGLX::Deinit()
{
	if (!m_display)
		return;
	
	// GL Context
	if (m_ctx)
	{
		glXDestroyContext(m_display, m_ctx);
		m_ctx = 0;
	}
	
	// pbuffer or window
	if (m_use_pbuffer)
	{
		if (m_pbuffer)
		{
			glXDestroyPbuffer(m_display, m_pbuffer);
			m_pbuffer = 0;
		}
	}
	else
	{
		if (m_win)
		{
			XDestroyWindow(m_display, m_win);
			m_win = 0;
		}
	}
	
	// XWindow
	if (m_display)
	{
		XCloseDisplay(m_display);
		m_display = 0;
	}
	
	KillX();
}

bool GLDeviceGLX::Resize(int width, int height)
{
  m_width = width;
  m_height = height;
	if (m_display)
	{
	    if (m_pbuffer)
	      {
			glXDestroyPbuffer(m_display, m_pbuffer);
			m_pbuffer = 0;
		}
		
		int attrib[] = {
			GLX_RENDER_TYPE, GLX_RGBA_BIT, //GLX_DOUBLEBUFFER,
			GLX_DRAWABLE_TYPE,  GLX_PBUFFER_BIT,
			GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8,
			GLX_DEPTH_SIZE, 24,
			None
		};
		Display* dpy = m_display;
		int scrnum = DefaultScreen( dpy );
		GLXFBConfig* configs;
		int nconfigs = 0;
		configs = glXChooseFBConfig(dpy, scrnum, attrib, &nconfigs);
		int pbattrs[] = {
			GLX_PBUFFER_WIDTH, width,
			GLX_PBUFFER_HEIGHT, height,
			0
		};
		m_pbuffer = glXCreatePbuffer(dpy, *configs, pbattrs);
		XFree(configs);

		if (!m_pbuffer)
		{
			printf("Error: glxCreatePbuffer\n");
			return false;
		}

		SetCurrent();
		glViewport(0, 0, width, height);		
	}
}
	
void GLDeviceGLX::SwapBuffers()
{
	if (m_display && m_win)
	{
		// TODO:
		
		SetCurrent();
	}
}

void GLDeviceGLX::SetCurrent()
{
    if (m_use_pbuffer)
        glXMakeCurrent(m_display, m_pbuffer, m_ctx);
    else
        glXMakeCurrent(m_display, m_win, m_ctx);
}

void GLDeviceGLX::UnsetCurrent()
{
    glXMakeCurrent(m_display, 0, 0);
}

	
#endif // Linux
