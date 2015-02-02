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

    //TODO Vec4Buffer
    //bool SetVec4Array(const std::string& name, Vec4Buffer* v);

    bool SetTranslate(float x, float y, float z)
    {
        m_trans[0] = x;
        m_trans[1] = y;
        m_trans[2] = z;
    }

    bool SetRotate(float x, float y, float z)
    {
        m_rotat[0] = x;
        m_rotat[1] = y;
        m_rotat[2] = z;
    }

    bool SetScale(float x, float y, float z)
    {
        m_scale[0] = x;
        m_scale[1] = y;
        m_scale[2] = z;
    }

    bool SetVec4(const std::string& name, float x, float y, float z, float w)
    {
        m_vec4s[name] = VX::Math::vec4(x,y,z,w);
    }

    bool SetVec3(const std::string& name, float x, float y, float z)
    {
        m_vec3s[name] = VX::Math::vec3(x,y,z);
    }

    bool SetVec2(const std::string& name, float x, float y)
    {
        m_vec2s[name] = VX::Math::vec2(x,y);
    }

    bool SetFloat(const std::string& name, float x)
    {
        m_floats[name] = x;
    }

protected:
    RenderObject(MODE_TYPE t) : m_type(t) {};
    ~RenderObject() {};
private:
    // Transform↲
    float           m_trans[3];
    float           m_rotat[3];
    float           m_scale[3];

    //mapped data
    std::map<std::string, VX::Math::vec4> m_vec4s;
    std::map<std::string, VX::Math::vec3> m_vec3s;
    std::map<std::string, VX::Math::vec2> m_vec2s;
    std::map<std::string, float> m_floats;
    std::map<std::string, int> m_ints;

    //mode type
    MODE_TYPE       m_type;
};

#endif //_RENDEROBJECT_H_
