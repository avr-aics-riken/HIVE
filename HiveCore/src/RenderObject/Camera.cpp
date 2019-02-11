/**
 * @file Camera.cpp
 * カメラ
 */
#include "Camera.h"
#include "BufferImageData.h"
#include <stdio.h>
#include <string>

/**
 * カメラクラス実装
 */
class Camera::Impl
{
  public:
	Impl()
	{
		m_width = 0;
		m_height = 0;
		m_outputfile = ""; // todo
        m_imagebuffer_format = "RGBA32F";
		m_clearcolor[0] = 0.0f;
		m_clearcolor[1] = 0.0f;
		m_clearcolor[2] = 0.0f;
		m_clearcolor[3] = 1.0f;
		m_imagebuffer = BufferImageData::CreateInstance();
		m_depthbuffer = BufferImageData::CreateInstance();
	}
	~Impl() {}

	/**
	 * スクリーンサイズの設定.
	 * @param w 幅
	 * @param h 高さ
	 */
	bool SetScreenSize(unsigned int w, unsigned int h)
	{
		m_width = w;
		m_height = h;
		return true;
	}

	/**
	 * 結果画像のファイルパスの設定.
	 * @param filename ファイルパス
	 */
	bool SetFilename(const std::string &filename)
	{
		m_outputfile = filename;
		// printf("Debug: FileName = %s\n", filename.c_str());
		return true;
	}

	/**
	 * 結果デプス画像のファイルパスの設定.
	 * @param filename ファイルパス
	 */
	bool SetDepthFilename(const std::string &filename)
	{
		m_depthoutputfile = filename;
		printf("Debug: Depth FileName = %s\n", filename.c_str());
		return true;
	}

	/**
	 * 結果画像の format の設定.
	 * @param format name of the format
	 */
	bool SetImageBufferFormat(const std::string &format)
	{
        if ((format.compare("RGBA8") == 0) ||
            (format.compare("RGBA32F") == 0)) {
            m_imagebuffer_format = format;
		    return true;
        } else {
		    fprintf(stderr, "[Camera] Unsupported image format name: %s\n", format.c_str());
		    return false;
        }
	}

	/**
	 * 視線の設定.
	 * @param eye_x 視点ベクトルx
	 * @param eye_y 視点ベクトルy
	 * @param eye_z 視点ベクトルz
	 * @param tar_x ターゲットベクトルx
	 * @param tar_y ターゲットベクトルy
	 * @param tar_z ターゲットベクトルz
	 * @param up_x アップベクトルx
	 * @param up_y アップベクトルy
	 * @param up_z アップベクトルz
	 * @param fov 縦方向視野角
	 */
	bool LookAt(float eye_x, float eye_y, float eye_z, float tar_x, float tar_y,
				float tar_z, float up_x, float up_y, float up_z, float fov)
	{
		CameraInfo *pinfo = &m_info;
		pinfo->eye[0] = eye_x;
		pinfo->eye[1] = eye_y;
		pinfo->eye[2] = eye_z;
		pinfo->tar[0] = tar_x;
		pinfo->tar[1] = tar_y;
		pinfo->tar[2] = tar_z;
		pinfo->up[0] = up_x;
		pinfo->up[1] = up_y;
		pinfo->up[2] = up_z;
		pinfo->fov = fov;
		return true;
	}

	/**
	 * クリアカラーの設定.
	 * @param red 赤
	 * @param green 緑
	 * @param blue 青
	 * @param alpha アルファ
	 */
	bool ClearColor(float red, float green, float blue, float alpha)
	{
		m_clearcolor[0] = red;
		m_clearcolor[1] = green;
		m_clearcolor[2] = blue;
		m_clearcolor[3] = alpha;
		return true;
	}

	/// 位置を返す
	VX::Math::vec3 GetPosition() const
	{
		return VX::Math::vec3(m_info.eye[0], m_info.eye[1], m_info.eye[2]);
	}
	/// ターゲットを返す
	VX::Math::vec3 GetTarget() const
	{
		return VX::Math::vec3(m_info.tar[0], m_info.tar[1], m_info.tar[2]);
	}
	/// アップベクトルを返す
	VX::Math::vec3 GetUp() const
	{
		return VX::Math::vec3(m_info.up[0], m_info.up[1], m_info.up[2]);
	}
	/// 視野角を返す
	float GetFov() const { return m_info.fov; }

	/// クリアカラーを返す.
	const float *GetClearColor() const { return m_clearcolor; }
	/// スクリーン幅を返す.
	int GetScreenWidth() const { return m_width; }
	/// スクリーン高さを返す.
	int GetScreenHeight() const { return m_height; }
	/// カメラ情報を返す.
	const Camera::CameraInfo *GetCameraInfo() const { return &m_info; }
	/// 出力ファイルパスを返す.
	const std::string &GetOutputFile() const { return m_outputfile; }
	/// 出力デプスファイルパスを返す.
	const std::string &GetDepthOutputFile() const { return m_depthoutputfile; }
	/// imagebuffer format を返す.
	const std::string &GetImageBufferFormat() const { return m_imagebuffer_format; }
	/// 画像バッファを返す.
	BufferImageData *GetImageBuffer() { return m_imagebuffer; }
	/// 深度バッファを返す.
	BufferImageData *GetDepthBuffer() { return m_depthbuffer; }
	
	/// 座標をスクリーンスペースに変換して返す
	VX::Math::vec3 ConvertToScreenSpace(float x, float y, float z)
	{
		VX::Math::vec4 p(x, y, z, 1.0);
		VX::Math::vec3 res;
		VX::Math::vec3 veye(m_info.eye);
		VX::Math::vec3 vtar(m_info.tar);
		VX::Math::vec3 vup(m_info.up);
		VX::Math::matrix4x4 viewmat = VX::Math::LookAt(veye, vtar, vup);
		const float width = static_cast<float>(GetScreenWidth());
		const float height = static_cast<float>(GetScreenHeight());
		const float aspect = width / height;
		VX::Math::matrix4x4 projectionmat = VX::Math::PerspectiveFov(m_info.fov, aspect, 0.1f, 500.0f);
		VX::Math::vec4 pp = projectionmat * (viewmat * p);
		float invw = 1.0f / pp.w;
		VX::Math::vec3 ndcPos(pp.x * invw, pp.y * invw, pp.z * invw);
		VX::Math::vec2 screenPos(
			(ndcPos.x + 1.0) / 2.0 * width,
			(ndcPos.y + 1.0) / 2.0 * height);
		return  VX::Math::vec3(screenPos.x, screenPos.y, 0.0f);
	}

  private:
	unsigned int m_width;
	unsigned int m_height;
	std::string m_outputfile;
	std::string m_depthoutputfile;
    std::string m_imagebuffer_format;
	CameraInfo m_info;
	float m_clearcolor[4];
	RefPtr<BufferImageData> m_imagebuffer;
	RefPtr<BufferImageData> m_depthbuffer;
};

/*

inline matrix4x4 PerspectiveFov(float fovy_degree, float Aspect, float zn, float zf)
{

	
inline matrix4x4 LookAt(const vec3& eye, const vec3& target, const vec3& up)
*/
Camera::Camera() : RenderObject(TYPE_CAMERA) { m_imp = new Impl(); }

Camera::~Camera() { delete m_imp; }

/**
 * スクリーンサイズの設定
 * @param w 幅
 * @param h 高さ
 */
bool Camera::SetScreenSize(unsigned int w, unsigned int h)
{
	return m_imp->SetScreenSize(w, h);
}

/**
 * 結果画像のファイルパスの設定
 * @param filename ファイルパス
 */
bool Camera::SetFilename(const std::string &filename)
{
	return m_imp->SetFilename(filename);
}

/**
 * 結果デプス画像のファイルパスの設定
 * @param filename ファイルパス
 */
bool Camera::SetDepthFilename(const std::string &filename)
{
	return m_imp->SetDepthFilename(filename);
}

/**
 * image buffer format の設定
 * @param format name of format
 */
bool Camera::SetImageBufferFormat(const std::string &format)
{
	return m_imp->SetImageBufferFormat(format);
}

/**
 * 視線の設定
 * @param eye_x 視点ベクトルx
 * @param eye_y 視点ベクトルy
 * @param eye_z 視点ベクトルz
 * @param tar_x ターゲットベクトルx
 * @param tar_y ターゲットベクトルy
 * @param tar_z ターゲットベクトルz
 * @param up_x アップベクトルx
 * @param up_y アップベクトルy
 * @param up_z アップベクトルz
 * @param fov 視野角
 */
bool Camera::LookAt(float eye_x, float eye_y, float eye_z, float tar_x,
					float tar_y, float tar_z, float up_x, float up_y,
					float up_z, float fov)
{
	return m_imp->LookAt(eye_x, eye_y, eye_z, tar_x, tar_y, tar_z, up_x, up_y,
						 up_z, fov);
}

/**
 * クリアカラーの設定
 * @param red 赤
 * @param green 緑
 * @param blue 青
 * @param alpha アルファ
 */
bool Camera::ClearColor(float red, float green, float blue, float alpha)
{
	return m_imp->ClearColor(red, green, blue, alpha);
}

/// クリアカラーを返す.
const float *Camera::GetClearColor() const { return m_imp->GetClearColor(); }
/// スクリーン幅を返す.
int Camera::GetScreenWidth() const { return m_imp->GetScreenWidth(); }
/// スクリーン高さを返す.
int Camera::GetScreenHeight() const { return m_imp->GetScreenHeight(); }
/// カメラ情報を返す.
const Camera::CameraInfo *Camera::GetCameraInfo() const
{
	return m_imp->GetCameraInfo();
}
/// 出力ファイルパスを返す.
const std::string &Camera::GetOutputFile() const
{
	return m_imp->GetOutputFile();
}
/// 出力デプスファイルパスを返す.
const std::string &Camera::GetDepthOutputFile() const
{
	return m_imp->GetDepthOutputFile();
}
/// image buffer format を返す.
const std::string &Camera::GetImageBufferFormat() const
{
	return m_imp->GetImageBufferFormat();
}

/// 画像バッファを返す.
BufferImageData *Camera::GetImageBuffer() { return m_imp->GetImageBuffer(); }
/// 深度バッファを返す.
BufferImageData *Camera::GetDepthBuffer() { return m_imp->GetDepthBuffer(); }

/// 位置を返す
VX::Math::vec3 Camera::GetPosition() const { return m_imp->GetPosition(); }
/// ターゲットを返す
VX::Math::vec3 Camera::GetTarget() const { return m_imp->GetTarget(); }
/// アップベクトルを返す
VX::Math::vec3 Camera::GetUp() const { return m_imp->GetUp(); }
/// 視野角を返す
float Camera::GetFov() const { return m_imp->GetFov(); }

/// 座標をスクリーンスペースに変換して返す
VX::Math::vec3 Camera::ConvertToScreenSpace(float x, float y, float z) {
	return m_imp->ConvertToScreenSpace(x, y, z);
}