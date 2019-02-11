#ifndef GLDEVICE_OSMESA_HEADER
#define GLDEVICE_OSMESA_HEADER

#if defined(_WIN32) || defined(_WIN64)
// win

#else

// linux
#define GL_GLEXT_PROTOTYPES
#include <GL/osmesa.h>
#include <GL/glu.h>
#include "GLDevice.h"

/*! @brief Cross platform GL Device Interface (GLX implementation)
 *
 * @note X Window OpenGL
 */
class GLDeviceOSMesa : public GLDevice
{
public:
	
	/*! @brief �R���X�g���N�^
	 */
	GLDeviceOSMesa()
	{
		m_use_pbuffer = true;
		m_ctx = NULL;
		screen_buffer = NULL;
		m_width = 0;
		m_height = 0;
	}

	/*! @brief �f�X�g���N�^
	 */
	~GLDeviceOSMesa()
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

	OSMesaContext m_ctx;
	void *screen_buffer;
	bool m_use_pbuffer;
	int m_width;
	int m_height;
};

/*! @brief GLDevice�̃C���X�^���X�쐬�֐�
 *  @return GLDevice�C���X�^���X�|�C���^��������
 */
inline GLDevice* CreateGLDeviceInstance()
{
	return new GLDeviceOSMesa();
}

	
#endif // Linux

#endif // GLDEVICE_OSMESA_HEADER

