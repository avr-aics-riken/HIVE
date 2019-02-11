#ifndef GLDEVICE_GLX_HEADER
#define GLDEVICE_GLX_HEADER

#if defined(_WIN32) || defined(_WIN64)
// win

#else

// linux
#include <X11/Xlib.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include "GLDevice.h"

/*! @brief Cross platform GL Device Interface (GLX implementation)
 *
 * @note X Window OpenGL
 */
class GLDeviceGLX : public GLDevice
{
public:
	
	/*! @brief �R���X�g���N�^
	 */
	GLDeviceGLX()
	{
		m_display = 0;
		m_win = 0;
		m_pbuffer = 0;
		m_use_pbuffer = true;
		m_ctx = 0;
		m_width = 0;
		m_height = 0;
	}

	/*! @brief �f�X�g���N�^
	 */
	~GLDeviceGLX()
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

	/*! @brief ���T�C�Y����
	 *  @return bool
	 */
	bool Resize(int width, int height);

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

	Display* m_display;
	Window m_win;
	GLXPbuffer m_pbuffer;
	bool m_use_pbuffer;
	GLXContext m_ctx;
	int m_width;
	int m_height;

};

/*! @brief GLDevice�̃C���X�^���X�쐬�֐�
 *  @return GLDevice�C���X�^���X�|�C���^��������
 */
inline GLDevice* CreateGLDeviceInstance()
{
	return new GLDeviceGLX();
}

	
	
#endif // Linux

#endif // GLDEVICE_GLX_HEADER

