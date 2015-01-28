#include "Camera.h"

#include <stdio.h>

Camera::Camera() : RenderObject(TYPE_CAMERA)
{
	m_width  = 0;
	m_height = 0;
	m_outputfile = ""; //todo
    m_clearcolor[0] = 0.0f;
    m_clearcolor[1] = 0.0f;
    m_clearcolor[2] = 0.0f;
    m_clearcolor[3] = 0.0f;
}

Camera::~Camera()
{
	//todo Clean?
}

bool Camera::SetScreenSize(unsigned int w, unsigned int h)
{
	m_width  = w;
	m_height = h;
	return true;
}

bool Camera::SetFilename(const std::string& filename)
{
	m_outputfile = filename;
	printf("Debug: FileName = %s\n", filename.c_str());
	return true;
}

bool Camera::LookAt(float eye_x, float eye_y, float eye_z,
				  float tar_x, float tar_y, float tar_z,
				  float up_x,  float up_y,  float up_z,
				  float fov)
{
	CameraInfo *pinfo = &m_info;
	pinfo->eye[0] = eye_x; pinfo->eye[1] = eye_y; pinfo->eye[2] = eye_z;
	pinfo->tar[0] = tar_x; pinfo->tar[1] = tar_y; pinfo->tar[2] = tar_z;
	pinfo->up [0] = up_x;  pinfo->up [1] = up_y;  pinfo->up [2] = up_z;
	pinfo->fov = fov;
	return true;
}

bool Camera::ClearColor(float red, float green, float blue, float alpha)
{
    m_clearcolor[0] = red;
    m_clearcolor[1] = green;
    m_clearcolor[2] = blue;
    m_clearcolor[3] = alpha;
}

const float* Camera::GetClearColor() const
{
    return m_clearcolor;
}

int Camera::GetScreenWidth() const
{
    return m_width;
}

int Camera::GetScreenHeight() const
{
    return m_height;
}

const Camera::CameraInfo* Camera::GetCameraInfo() const
{
    return &m_info;
}

const std::string& Camera::GetOutputFile() const
{
    return m_outputfile;
}


