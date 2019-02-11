#ifndef GLDEVICE_WGL_HEADER
#define GLDEVICE_WGL_HEADER

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/wglext.h>
#include "GLDevice.h"

/*! @brief Cross platform GL Device Interface (WGL implementation)
 *
 * @note Windows OpenGL
 */
class GLDeviceWGL : public GLDevice
{
public:
	/*! @brief �R���X�g���N�^
	 */
	GLDeviceWGL() : GLDevice()
	{
		m_hDC = NULL;
		m_hRC = NULL;
		m_hWnd = NULL;

		m_pbuffer_width = 0;
		m_pbuffer_height = 0;
		m_hPbufferARB = NULL;
	}
	/*! @brief �f�X�g���N�^
	 */
	~GLDeviceWGL()
	{
		Deinit();
	}

	/*! @brief ����������
	 *  @param[in] width ��
	 *  @param[in] height ����
	 *  @param[in] colorbit �F�r�b�g��
	 *  @param[in] depthbit �f�v�X�r�b�g��
	 *  @param[in] use_pbuffer pbuffer�̎g�p
	 *  @return ���������ꍇtrue, ���s�����ꍇfalse
	 */
	bool Init(int width, int height, int colorbit, int depthbit, bool use_pbuffer = false);

	/*! @brief �I������
	 *  @return void
	 */
	void Deinit();

	/*! @brief SwapBuffers�R�}���h
	 *  @return void
	 */
	void SwapBuffers();

	/*! @brief OpenGL�̃f�o�C�X�R���e�L�X�g���X���b�h�Ɋ֘A�t������
	 *  @return void
	 */
	void SetCurrent();

	/*! @brief OpenGL�̃f�o�C�X�R���e�L�X�g���X���b�h�Ɋ֘A�t�����Ȃ�
	 *  @return void
	 */
	void UnsetCurrent();

private:
	bool initWGL();

	HDC m_hDC;
	HGLRC m_hRC;
	HWND m_hWnd;

	int m_pbuffer_width;
	int m_pbuffer_height;
	HPBUFFERARB m_hPbufferARB;
};

/*! @brief GLDevice�̃C���X�^���X�쐬�֐�
 *  @return GLDevice�C���X�^���X�|�C���^��������
 */
inline GLDevice* CreateGLDeviceInstance()
{
	return new GLDeviceWGL();
}


#endif // Windows

#endif // GLDEVICE_WGL_HEADER

