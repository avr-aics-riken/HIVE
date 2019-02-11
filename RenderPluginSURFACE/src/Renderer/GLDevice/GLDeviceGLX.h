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
	
	/*! @brief コンストラクタ
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

	/*! @brief デストラクタ
	 */
	~GLDeviceGLX()
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

	/*! @brief リサイズ処理
	 *  @return bool
	 */
	bool Resize(int width, int height);

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

	Display* m_display;
	Window m_win;
	GLXPbuffer m_pbuffer;
	bool m_use_pbuffer;
	GLXContext m_ctx;
	int m_width;
	int m_height;

};

/*! @brief GLDeviceのインスタンス作成関数
 *  @return GLDeviceインスタンスポインタがかえる
 */
inline GLDevice* CreateGLDeviceInstance()
{
	return new GLDeviceGLX();
}

	
	
#endif // Linux

#endif // GLDEVICE_GLX_HEADER

