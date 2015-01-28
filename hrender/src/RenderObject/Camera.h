#ifndef _CAMERA_H_
#define _CAMERA_H_


//ref::PolygonModel.h

#include "RenderObject.h"
#include <string>

class Camera : public RenderObject {
    
public:
    struct CameraInfo {
        float eye[3];
        float tar[3];
        float up [3];
        float fov;
    };
    
    const float* GetClearColor() const;
    int GetScreenWidth() const;
    int GetScreenHeight() const;
    const CameraInfo* GetCameraInfo() const;
    const std::string& GetOutputFile() const;
    
private:
	unsigned int m_width;
	unsigned int m_height;
	std::string  m_outputfile;
	CameraInfo m_info;
    float m_clearcolor[4];
    
protected:
	Camera();
	~Camera();
	bool SetScreenSize(unsigned int width, unsigned int height);
	bool SetFilename(const std::string& outputfile);
	bool LookAt(float eye_x, float eye_y, float eye_z,
				float tar_x, float tar_y, float tar_z,
				float up_x,  float up_y,  float up_z,
				float fov);
    bool ClearColor(float red, float green, float blue, float alpha);

};

#endif //_CAMERA_H_
