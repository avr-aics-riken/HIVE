#if defined(_WIN32) || defined(_WIN64)
// win

#else
// linux

#include "GLDeviceOSMesa.h"
#include "GLDeviceExtention.h"
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>


namespace
{

};// namespace
				 
				 
bool GLDeviceOSMesa::Init(int width, int height, int colorbit, int depthbit, bool use_pbuffer)
{
	m_ctx = OSMesaCreateContextExt( OSMESA_RGBA, depthbit, 0, 0, NULL );

	if(!m_ctx)
	{
		printf("Error: create OSMesa context \n");
		return false;
	}
	m_width = width;
	m_height = height;

	screen_buffer = malloc( m_width * m_height * 4 * sizeof(GLubyte));
	if(screen_buffer == NULL)
	{
		printf("Error: Alloc screen buffer faild\n");
		return false;
	}
	if(!OSMesaMakeCurrent( m_ctx, screen_buffer, GL_UNSIGNED_BYTE, m_width, m_height))
	{
		printf("Error: Set OSMesa Context\n");
		return false;
	}

	if (InitGLExtention() == false)
	{
		printf("Error: not supported GL ExtentionsÅ_n");
		return false;
	}

	int z, s, a;
	glGetIntegerv(GL_DEPTH_BITS, &z);
	glGetIntegerv(GL_STENCIL_BITS, &s);
	glGetIntegerv(GL_ACCUM_RED_BITS, &a);

	return true;
}
	
void GLDeviceOSMesa::Deinit()
{
	// GL Context
	if (m_ctx)
	{
		OSMesaDestroyContext(m_ctx);
		m_ctx = NULL;
	}
	if(screen_buffer != NULL)
	{
		free(screen_buffer);
		screen_buffer = NULL;
	}
}
	
void GLDeviceOSMesa::SwapBuffers()
{
	
//	glXSwapBuffers(in);
}

void GLDeviceOSMesa::SetCurrent()
{

	OSMesaMakeCurrent( m_ctx, screen_buffer, GL_UNSIGNED_BYTE, m_width, m_height);
}

void GLDeviceOSMesa::UnsetCurrent()
{
	OSMesaMakeCurrent( 0, 0, 0, 0, 0);
}

	
#endif // Linux
