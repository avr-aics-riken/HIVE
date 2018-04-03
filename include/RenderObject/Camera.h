/**
 * @file Camera.h
 * カメラクラス
 */
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "RenderObject.h"

class BufferImageData;

/**
 * カメラクラス
 */
class Camera : public RenderObject
{

  public:
	/**
	 * カメラ情報.
	 */
	struct CameraInfo
	{
		float eye[3];
		float tar[3];
		float up[3];
		float fov;
	};

	const float *GetClearColor() const;
	int GetScreenWidth() const;
	int GetScreenHeight() const;
	const CameraInfo *GetCameraInfo() const;
	const std::string &GetOutputFile() const;
	const std::string &GetDepthOutputFile() const;
	const std::string &GetImageBufferFormat() const;
	BufferImageData *GetImageBuffer();
	BufferImageData *GetDepthBuffer();

	VX::Math::vec3 GetPosition() const;
	VX::Math::vec3 GetTarget() const;
	VX::Math::vec3 GetUp() const;

    float GetFov() const;
  private:
	class Impl;
	Impl *m_imp;

  protected:
	// Script Access Only
	Camera();
	~Camera();
	bool SetScreenSize(unsigned int width, unsigned int height);
	bool SetFilename(const std::string &outputfile);
  bool SetDepthFilename(const std::string& outputfile);
	/// Specify image buffer format by string.
	/// Currently, either "RGBA8" or "RGBA32F" are supported.
	bool SetImageBufferFormat(const std::string &format);

	bool LookAt(float eye_x, float eye_y, float eye_z, float tar_x, float tar_y,
				float tar_z, float up_x, float up_y, float up_z, float fov);
	bool ClearColor(float red, float green, float blue, float alpha);
	
	VX::Math::vec3 ConvertToScreenSpace(float x, float y, float z);
	bool GetBox(VX::Math::vec3& bmin, VX::Math::vec3& bmax) const { return false; }
};

#endif //_CAMERA_H_
