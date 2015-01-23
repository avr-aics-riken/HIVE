#ifndef _CAMERA_H_
#define _CAMERA_H_


//ref::PolygonModel.h

#include "RenderObject.h"
#include <string>

class Camera : public RenderObject {
	unsigned int width;
	unsigned int height;
	std::string  outputfile;
	struct CameraInfo {
		float eye[3];
		float tar[3];
		float up [3];
		float fov;
	};
	CameraInfo info;
protected:
	Camera();
	~Camera();
	bool SetScreenSize(unsigned int width, unsigned int height);
	bool SetFilename(const std::string& outputfile);
	bool LookAt(float eye_x, float eye_y, float eye_z,
				float tar_x, float tar_y, float tar_z,
				float up_x,  float up_y,  float up_z,
				float fov);
};

#endif //_CAMERA_H_
