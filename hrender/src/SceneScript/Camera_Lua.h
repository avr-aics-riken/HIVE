#ifndef _CAMERA_LUA_H_
#define _CAMERA_LUA_H_


//ref::PolygonModel.h

#include "RenderObject.h"
#include "LuaUtil.h"
#include "Camera.h"

class Camera_Lua : public Camera {
public:
	Camera_Lua(){}
	~Camera_Lua(){}
    bool SetScreenSize(unsigned int width, unsigned int height);
    bool SetFilename(const std::string& outputfile);
	bool LookAt(float eye_x, float eye_y, float eye_z,
					  float tar_x, float tar_y, float tar_z,
					  float up_x,  float up_y,  float up_z,
					  float fov);
	
    LUA_SCRIPTCLASS_BEGIN(Camera_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool, SetScreenSize, unsigned int, unsigned int)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetFilename, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG10(bool, LookAt, float, float, float, float, float, float, float, float, float, float)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(Camera_Lua);

#endif //_CAMERA_LUA_H_
