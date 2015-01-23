#include "Camera_Lua.h"

bool Camera_Lua::SetScreenSize(unsigned int w, unsigned int h)
{
	return Camera::SetScreenSize(w, h);
}

bool Camera_Lua::SetFilename(const std::string& filename)
{
	return Camera::SetFilename(filename);
}

bool Camera_Lua::LookAt(float eye_x, float eye_y, float eye_z,
					float tar_x, float tar_y, float tar_z,
					float up_x,  float up_y,  float up_z,
					float fov)
{
	return Camera::LookAt(
	  eye_x, eye_y, eye_z,
	  tar_x, tar_y, tar_z,
	  up_x,  up_y,  up_z,
	  fov);
}
