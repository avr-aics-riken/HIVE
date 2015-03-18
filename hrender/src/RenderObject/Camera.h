/**
 * @file Camera.h
 * カメラ
 */
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "RenderObject.h"

class BufferImageData;

/**
 * カメラ.
 */
class Camera : public RenderObject {
    
public:
    /**
     * カメラ情報.
     */
    struct CameraInfo {
        float eye[3];
        float tar[3];
        float up [3];
        float fov;
    };
    
    const float*       GetClearColor() const;
    int                GetScreenWidth() const;
    int                GetScreenHeight() const;
    const CameraInfo*  GetCameraInfo() const;
    const std::string& GetOutputFile() const;
    BufferImageData*   GetImageBuffer();
    BufferImageData*   GetDepthBuffer();

    VX::Math::vec3 GetPosition() const;
    VX::Math::vec3 GetTarget() const;
    VX::Math::vec3 GetUp() const;
    
private:
    class Impl;
    Impl* m_imp;
    
protected:
    // Script Access Only
	Camera();
	~Camera();
	bool SetScreenSize(unsigned int width, unsigned int height);
	bool SetFilename(const std::string& outputfile);
    
	bool LookAt(float eye_x, float eye_y, float eye_z,
				float tar_x, float tar_y, float tar_z,
				float up_x,  float up_y,  float up_z,
				float fov);
    bool ClearColor(float red, float green, float blue, float alpha);
    float GetFov() const;
};

#endif //_CAMERA_H_
