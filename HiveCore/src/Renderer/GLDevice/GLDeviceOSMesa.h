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
	
	/*! @brief コンストラクタ
	 */
	GLDeviceOSMesa()
	{
		m_use_pbuffer = true;
		m_ctx = NULL;
		screen_buffer = NULL;
		m_width = 0;
		m_height = 0;
	}

	/*! @brief デストラクタ
	 */
	~GLDeviceOSMesa()
	{
		Deinit();
	}

	/*! @brief 初期化処理
	 *  @param[in] width 幅
	 *  @param[in] height 高さ
	 *  @param[in] colorbit 色ビット数
	 *  @param[in] depthbit デプスビット数
	 *  @param[in] use_pbuffer pbufferの使用
	 *  @return 成功した場合true, 失敗した場合false
	 */
	bool Init(int width, int height, int colorbit, int depthbit, bool use_pbuffer = false);

	/*! @brief 終了処理
	 *  @return void
	 */
	void Deinit();

	/*! @brief SwapBuffersコマンド
	 *  @return void
	 */
	void SwapBuffers();

	/*! @brief OpenGLのデバイスコンテキストをスレッドに関連付けする
	 *  @return void
	 */
	void SetCurrent();

	/*! @brief OpenGLのデバイスコンテキストをスレッドに関連付けしない
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

/*! @brief GLDeviceのインスタンス作成関数
 *  @return GLDeviceインスタンスポインタがかえる
 */
inline GLDevice* CreateGLDeviceInstance()
{
	return new GLDeviceOSMesa();
}

	
#endif // Linux

#endif // GLDEVICE_OSMESA_HEADER

