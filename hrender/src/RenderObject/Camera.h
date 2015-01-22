#ifndef _CAMERA_H_
#define _CAMERA_H_


//ref::PolygonModel.h

#include "RenderObject.h"
#include "LuaUtil.h"

class Camera : public RenderObject {
    unsigned int width;
    unsigned int height;
    std::string  outputfile;
public:
	struct CameraInfo {
		float eye[3];
		float tar[3];
		float up [3];
		float fov;
	};
	CameraInfo info;

    Camera();
    ~Camera();
	
	CameraInfo* GetCameraInfo() const;
	
	//Lua
    bool SetScreenSize(unsigned int width, unsigned int height);
    bool SetFilename(const std::string& outputfile);
	bool LookAt(float eye_x, float eye_y, float eye_z,
					  float tar_x, float tar_y, float tar_z,
					  float up_x,  float up_y,  float up_z,
					  float fov);
	
    LUA_SCRIPTCLASS_BEGIN(Camera)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool, SetScreenSize, unsigned int, unsigned int)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetFilename, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG10(bool, LookAt, float, float, float, float, float, float, float, float, float, float)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(Camera);

#endif //_CAMERA_H_
