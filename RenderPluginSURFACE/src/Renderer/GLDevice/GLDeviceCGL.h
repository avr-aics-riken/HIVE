#ifndef GLDEVICE_CGL_HEADER
#define GLDEVICE_CGL_HEADER

#if defined(__APPLE__) || defined(MACOSX)

// MacOSX
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "GLDevice.h"

/*! @brief Cross platform GL Device Interface (CGL implementation)
 *
 * @note Core OpenGL (for MacOSX)
 */
class GLDeviceCGL : public GLDevice
{
public:
	
	/*! @brief コンストラクタ
	 */
	GLDeviceCGL();

	/*! @brief デストラクタ
	 */
	~GLDeviceCGL();

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
	 *  @return void
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
	
	static GLDevice* CreateGLDeviceInstance();
private:

	//CGLWindowObj  m_win;
	CGLPBufferObj m_pbuffer;
	CGLContextObj m_ctx;
	bool m_use_pbuffer;
	int m_width;
	int m_height;

};

/*! @brief GLDeviceのインスタンス作成関数
 *  @return GLDeviceインスタンスポインタがかえる
 */
GLDevice* CreateGLDeviceInstance();

	
	
#endif // MacOSX

#endif // GLDEVICE_CGL_HEADER

