/*
   RenderObject.h
   */
#ifndef _RENDEROBJECT_H_
#define _RENDEROBJECT_H_

#include "../Core/Ref.h"
#include "../Core/vxmath.h"
#include <string>
#include <map>

class RenderObject : public RefCount
{
public:
    enum MODE_TYPE {
        TYPE_NONE,
        TYPE_SETTING,
        TYPE_CAMERA,
        TYPE_VOLUME,
        TYPE_POLYGON,
        TYPE_POINT,
        TYPE_MAX,
    };

    MODE_TYPE GetType() const {
        return m_type;
    }

    //--------------------------------------------------
    //Get
    //--------------------------------------------------
    const VX::Math::vec3&  GetTranslate() const { return m_trans; }
    const VX::Math::vec3&  GetRotate()    const { return m_rotat; }
    const VX::Math::vec3&  GetScale()     const { return m_scale; }
    
    typedef std::map<std::string, VX::Math::vec4> Vec4Map;
    typedef std::map<std::string, VX::Math::vec3> Vec3Map;
    typedef std::map<std::string, VX::Math::vec2> Vec2Map;
    typedef std::map<std::string, float>         FloatMap;
    const Vec4Map&  GetUniformVec4 () const { return m_vec4s;  }
    const Vec3Map&  GetUniformVec3 () const { return m_vec3s;  }
    const Vec2Map&  GetUniformVec2 () const { return m_vec2s;  }
    const FloatMap& GetUniformFloat() const { return m_floats; }

protected:
    //--------------------------------------------------
    //Set
    //--------------------------------------------------
    void SetTranslate(float x, float y, float z) {
        m_trans[0] = x;
        m_trans[1] = y;
        m_trans[2] = z;
    }
 

    void SetRotate(float x, float y, float z) {
        m_rotat[0] = x;
        m_rotat[1] = y;
        m_rotat[2] = z;
    }

    void SetScale(float x, float y, float z) {
        m_scale[0] = x;
        m_scale[1] = y;
        m_scale[2] = z;
    }

    void SetVec4(const std::string& name, float x, float y, float z, float w) {
        m_vec4s[name] = VX::Math::vec4(x,y,z,w);
    }

    void SetVec3(const std::string& name, float x, float y, float z) {
        m_vec3s[name] = VX::Math::vec3(x,y,z);
    }

    void SetVec2(const std::string& name, float x, float y) {
        m_vec2s[name] = VX::Math::vec2(x,y);
    }

    void SetFloat(const std::string& name, float x) {
        m_floats[name] = x;
    }

    RenderObject(MODE_TYPE t) : m_type(t) 
    {
        m_trans =  VX::Math::vec3(0, 0, 0);
        m_rotat =  VX::Math::vec3(0, 0, 0);
        m_scale =  VX::Math::vec3(1, 1, 1);
    };
    ~RenderObject() {};
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

    //mode type
    MODE_TYPE       m_type;
};

#endif //_RENDEROBJECT_H_
