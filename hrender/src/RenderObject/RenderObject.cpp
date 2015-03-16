#include "RenderObject.h"
#include "../Buffer/BufferImageData.h"

class RenderObject::Impl
{
public:
    RenderObject::MODE_TYPE GetType() const {
        return m_type;
    }

    std::string GetTypeString() const {
        static const char* modeString[] = {
            "NONE",
            "SETTING",
            "CAMERA",
            "VOLUME",
            "POLYGON",
            "LINE",
            "POINT",
            "TETRA",
            "VECTOR"
        };
        return std::string(modeString[m_type]);
    }

    //--------------------------------------------------
    //Get
    //--------------------------------------------------
    const VX::Math::vec3&  GetTranslate() const { return m_trans; }
    const VX::Math::vec3&  GetRotate()    const { return m_rotat; }
    const VX::Math::vec3&  GetScale()     const { return m_scale; }
    const VX::Math::matrix GetTransformMatrix() const
    {
        return VX::Math::Translation(m_trans[0], m_trans[1], m_trans[2])
             * VX::Math::RotationYawPitchRoll(m_rotat[1], m_rotat[0], m_rotat[2])
             * VX::Math::Scaling(m_scale[0], m_scale[1], m_scale[2]); // Y,X,Z
    }
    
    typedef std::map<std::string, VX::Math::vec4> Vec4Map;
    typedef std::map<std::string, VX::Math::vec3> Vec3Map;
    typedef std::map<std::string, VX::Math::vec2> Vec2Map;
    typedef std::map<std::string, float>         FloatMap;
    typedef std::map<std::string, RefPtr<const BufferImageData> > TextureMap;
    const Vec4Map&    GetUniformVec4 ()   const { return m_vec4s;  }
    const Vec3Map&    GetUniformVec3 ()   const { return m_vec3s;  }
    const Vec2Map&    GetUniformVec2 ()   const { return m_vec2s;  }
    const FloatMap&   GetUniformFloat()   const { return m_floats; }
    const TextureMap& GetUniformTexture() const { return m_imgs;   }

    //--------------------------------------------------
    //Set
    //--------------------------------------------------
    bool SetTranslate(float x, float y, float z) {
        m_trans[0] = x;
        m_trans[1] = y;
        m_trans[2] = z;
        return true;
    }
    
    bool SetRotate(float x, float y, float z) {
        m_rotat[0] = x;
        m_rotat[1] = y;
        m_rotat[2] = z;
        return true;
    }

    bool SetScale(float x, float y, float z) {
        m_scale[0] = x;
        m_scale[1] = y;
        m_scale[2] = z;
        return true;
    }

    bool SetVec4(const std::string& name, float x, float y, float z, float w) {
        m_vec4s[name] = VX::Math::vec4(x,y,z,w);
        return true;
    }

    bool SetVec3(const std::string& name, float x, float y, float z) {
        m_vec3s[name] = VX::Math::vec3(x,y,z);
        return true;
    }

    bool SetVec2(const std::string& name, float x, float y) {
        m_vec2s[name] = VX::Math::vec2(x,y);
        return true;
    }

    bool SetFloat(const std::string& name, float x) {
        m_floats[name] = x;
        return true;
    }
    
    bool SetTexture(const std::string& name, const BufferImageData* img) {
        m_imgs[name] = img;
        return true;
    }

    
    Impl(MODE_TYPE t) : m_type(t)
    {
        m_trans =  VX::Math::vec3(0, 0, 0);
        m_rotat =  VX::Math::vec3(0, 0, 0);
        m_scale =  VX::Math::vec3(1, 1, 1);
    };
    ~Impl() {};
    
private:
    // Transform↲
    VX::Math::vec3 m_trans;
    VX::Math::vec3 m_rotat;
    VX::Math::vec3 m_scale;

    //mapped data
    std::map<std::string, VX::Math::vec4> m_vec4s;
    std::map<std::string, VX::Math::vec3> m_vec3s;
    std::map<std::string, VX::Math::vec2> m_vec2s;
    std::map<std::string, float>          m_floats;
    std::map<std::string, int>            m_ints;
    std::map<std::string, RefPtr<const BufferImageData> > m_imgs;

    //mode type
    RenderObject::MODE_TYPE m_type;
};

RenderObject::RenderObject(MODE_TYPE t) : m_imp(new Impl(t)) {}
RenderObject::~RenderObject() { delete m_imp; }

RenderObject::MODE_TYPE RenderObject::GetType() const { return m_imp->GetType();       }
std::string RenderObject::GetTypeString()       const { return m_imp->GetTypeString(); }

//--------------------------------------------------
//Get
//--------------------------------------------------
const VX::Math::vec3&  RenderObject::GetTranslate()               const { return m_imp->GetTranslate();       }
const VX::Math::vec3&  RenderObject::GetRotate()                  const { return m_imp->GetRotate();          }
const VX::Math::vec3&  RenderObject::GetScale()                   const { return m_imp->GetScale();           }
const VX::Math::matrix RenderObject::GetTransformMatrix()         const { return m_imp->GetTransformMatrix(); }
const RenderObject::Vec4Map&  RenderObject::GetUniformVec4 ()     const { return m_imp->GetUniformVec4();     }
const RenderObject::Vec3Map&  RenderObject::GetUniformVec3 ()     const { return m_imp->GetUniformVec3();     }
const RenderObject::Vec2Map&  RenderObject::GetUniformVec2 ()     const { return m_imp->GetUniformVec2();     }
const RenderObject::FloatMap& RenderObject::GetUniformFloat()     const { return m_imp->GetUniformFloat();    }
const RenderObject::TextureMap& RenderObject::GetUniformTexture() const { return m_imp->GetUniformTexture();  }

//--------------------------------------------------
//Set
//--------------------------------------------------
bool RenderObject::SetTranslate(float x, float y, float z)                              { return m_imp->SetTranslate(x, y, z);     }
bool RenderObject::SetRotate(float x, float y, float z)                                 { return m_imp->SetRotate(x, y, z);        }
bool RenderObject::SetScale(float x, float y, float z)                                  { return m_imp->SetScale(x, y, z);         }
bool RenderObject::SetVec4(const std::string& name, float x, float y, float z, float w) { return m_imp->SetVec4(name, x, y, z, w); }
bool RenderObject::SetVec3(const std::string& name, float x, float y, float z)          { return m_imp->SetVec3(name, x, y, z);    }
bool RenderObject::SetVec2(const std::string& name, float x, float y)                   { return m_imp->SetVec2(name, x, y);       }
bool RenderObject::SetFloat(const std::string& name, float x)                           { return m_imp->SetFloat(name, x);         }
bool RenderObject::SetTexture(const std::string& name, const BufferImageData* img)      { return m_imp->SetTexture(name, img);     }


