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
	/*! @brief コンストラクタ
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
	/*! @brief デストラクタ
	 */
	~GLDeviceWGL()
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
	bool initWGL();

	HDC m_hDC;
	HGLRC m_hRC;
	HWND m_hWnd;

	int m_pbuffer_width;
	int m_pbuffer_height;
	HPBUFFERARB m_hPbufferARB;
};

/*! @brief GLDeviceのインスタンス作成関数
 *  @return GLDeviceインスタンスポインタがかえる
 */
inline GLDevice* CreateGLDeviceInstance()
{
	return new GLDeviceWGL();
}


#endif // Windows

#endif // GLDEVICE_WGL_HEADER

