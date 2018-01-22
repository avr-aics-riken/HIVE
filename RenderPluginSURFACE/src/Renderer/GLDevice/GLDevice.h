#ifndef GLDEVICE_HEADER
#define GLDEVICE_HEADER


/*! @brief Cross platform GL Device 抽象Interface
 *
 * @note PBuffer or Window host
 * この抽象インターフェースはインスタンスを作成することはできない
 */
class GLDevice
{
public:

	/*! @brief 初期化処理
	 *  @param[in] width 幅
	 *  @param[in] height 高さ
	 *  @param[in] colorbit 色ビット数
	 *  @param[in] depthbit デプスビット数
	 *  @param[in] use_pbuffer pbufferの使用
	 *  @return 成功した場合true, 失敗した場合false
	 */
	virtual bool Init(int width, int height,
		int colorbit, int depthbit, bool use_pbuffer = false) = 0;

	/*! @brief 終了処理
	 *  @return void
	 */
	virtual void Deinit() = 0;

	/*! @brief SwapBuffersコマンド
	 *  @return void
	 */
	virtual void SwapBuffers() = 0;

	/*! @brief リサイズ処理
	 *  @return bool
	 */
	virtual bool Resize(int width, int height) = 0;

	/*! @brief デストラクタ
	 */
	virtual ~GLDevice(){};

	/*! @brief OpenGLのデバイスコンテキストをスレッドに関連付けする
	 *  @return void
	 */
	virtual void SetCurrent() = 0;

	/*! @brief OpenGLのデバイスコンテキストをスレッドに関連付けしない
	 *  @return void
	 */
	virtual void UnsetCurrent() = 0;
protected:
	/*! @brief コンストラクタ
	 * 
	 * @note 抽象interface. インスタンスの作成を禁止
	 */
	GLDevice(){}
};


#if defined(_WIN32) || defined(_WIN64)
// Windows

#include "GLDeviceWGL.h"

#elif defined(__APPLE__) || defined(MACOSX)
// MacOSX

#include "GLDeviceCGL.h"

#else
// Linux

#ifdef USE_OSMesa
#include "GLDeviceOSMesa.h"
#else
#include "GLDeviceGLX.h"
#endif /* USE_OSMesa */

#endif


#endif // GLDEVICE_HEADER

