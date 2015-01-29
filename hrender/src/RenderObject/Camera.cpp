
#include <stdio.h>
#include <string>
#include "Camera.h"

class Camera::Impl
{
public:
    Impl()
    {
        m_width  = 0;
        m_height = 0;
        m_outputfile = ""; //todo
        m_clearcolor[0] = 0.0f;
        m_clearcolor[1] = 0.0f;
        m_clearcolor[2] = 0.0f;
        m_clearcolor[3] = 0.0f;
    }
    ~Impl()
    {
        
    }
    
    
    bool SetScreenSize(unsigned int w, unsigned int h)
    {
        m_width  = w;
        m_height = h;
        return true;
    }
    
    bool SetFilename(const std::string& filename)
    {
        m_outputfile = filename;
        printf("Debug: FileName = %s\n", filename.c_str());
        return true;
    }
    
    bool SetNetworkAddress(const std::string& address)
    {
        m_networkaddress = address;
        printf("Debug: Network = %s\n", address.c_str());
        return true;
    }
    
    bool LookAt(float eye_x, float eye_y, float eye_z,
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
    
    bool ClearColor(float red, float green, float blue, float alpha)
    {
        m_clearcolor[0] = red;
        m_clearcolor[1] = green;
        m_clearcolor[2] = blue;
        m_clearcolor[3] = alpha;
        return true;
    }
    
    const float*              GetClearColor()   const { return m_clearcolor; }
    int                       GetScreenWidth()  const { return m_width;      }
    int                       GetScreenHeight() const { return m_height;     }
    const Camera::CameraInfo* GetCameraInfo()   const { return &m_info;      }
    const std::string&        GetOutputFile()   const { return m_outputfile; }

private:
    unsigned int m_width;
    unsigned int m_height;
    std::string  m_outputfile;
    std::string  m_networkaddress;
    CameraInfo m_info;
    float m_clearcolor[4];

};

Camera::Camera() : RenderObject(TYPE_CAMERA)
{
    m_imp = new Impl();
}

Camera::~Camera()
{
    delete m_imp;
}

bool Camera::SetScreenSize(unsigned int w, unsigned int h)
{
    return m_imp->SetScreenSize(w, h);
}

bool Camera::SetFilename(const std::string& filename)
{
    return m_imp->SetFilename(filename);
}

bool Camera::SetNetworkAddress(const std::string& address)
{
    return m_imp->SetNetworkAddress(address);
}

bool Camera::LookAt(float eye_x, float eye_y, float eye_z,
				  float tar_x, float tar_y, float tar_z,
				  float up_x,  float up_y,  float up_z,
				  float fov)
{
	return m_imp->LookAt(eye_x, eye_y, eye_z,
                         tar_x, tar_y, tar_z,
                         up_x , up_y , up_z ,
                         fov);
}

bool Camera::ClearColor(float red, float green, float blue, float alpha)
{
    return m_imp->ClearColor(red, green, blue, alpha);
}

const float*              Camera::GetClearColor()   const { return m_imp->GetClearColor();   }
int                       Camera::GetScreenWidth()  const { return m_imp->GetScreenWidth();  }
int                       Camera::GetScreenHeight() const { return m_imp->GetScreenHeight(); }
const Camera::CameraInfo* Camera::GetCameraInfo()   const { return m_imp->GetCameraInfo();   }
const std::string&        Camera::GetOutputFile()   const { return m_imp->GetOutputFile();   }


