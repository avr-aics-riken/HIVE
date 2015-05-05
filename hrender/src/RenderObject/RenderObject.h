/**
 * @file RenderObject.h
 * レンダーオブジェクト
 */

#ifndef _RENDEROBJECT_H_
#define _RENDEROBJECT_H_

#include "../Core/Ref.h"
#include "../Core/vxmath.h"
#include <string>
#include <vector>
#include <map>

class BufferImageData;

/**
 * レンダーオブジェクト
 */
class RenderObject : public RefCount
{
public:
    enum MODE_TYPE {
        TYPE_NONE = 0,
        TYPE_SETTING,
        TYPE_CAMERA,
        TYPE_VOLUME,
        TYPE_SPARSEVOLUME,
        TYPE_POLYGON,
        TYPE_LINE,
        TYPE_POINT,
        TYPE_TETRA,
        TYPE_VECTOR,
        TYPE_MAX,
    };

    MODE_TYPE GetType() const;
    std::string GetTypeString() const;

    //--------------------------------------------------
    //Get
    //--------------------------------------------------
    const VX::Math::vec3&  GetTranslate() const;
    const VX::Math::vec3&  GetRotate()    const;
    const VX::Math::vec3&  GetScale()     const;
    const VX::Math::matrix GetTransformMatrix() const;
    
    typedef std::map<std::string, VX::Math::vec4>                 Vec4Map;
    typedef std::map<std::string, VX::Math::vec3>                 Vec3Map;
    typedef std::map<std::string, VX::Math::vec2>                 Vec2Map;
    typedef std::map<std::string, float>                          FloatMap;
    typedef std::map<std::string, RefPtr<const BufferImageData> > TextureMap;
    typedef std::map<std::string, bool>                           FilteringParamMap;
    typedef std::map<std::string, std::vector<bool> >             WrappingParamMap; // <S, T, R>
    const Vec4Map&    GetUniformVec4 ()   const;
    const Vec3Map&    GetUniformVec3 ()   const;
    const Vec2Map&    GetUniformVec2 ()   const;
    const FloatMap&   GetUniformFloat()   const;
    const TextureMap& GetUniformTexture() const;
    const FilteringParamMap& GetTextureFiltering() const;
    const WrappingParamMap& GetTextureWrapping() const;


protected:
    //--------------------------------------------------
    //Set
    //--------------------------------------------------
    bool SetTranslate(float x, float y, float z);
    bool SetRotate(float x, float y, float z);
    bool SetScale(float x, float y, float z);
    bool SetTransformMatrix(const float* m);

    bool SetVec4(const std::string& name, float x, float y, float z, float w);
    bool SetVec3(const std::string& name, float x, float y, float z);
    bool SetVec2(const std::string& name, float x, float y);
    bool SetFloat(const std::string& name, float x);
    bool SetTexture(const std::string& name, const BufferImageData* img);
    bool SetTextureFiltering(const std::string& name, bool filter);
    /// Set texture wrapping mode. CLAMP_TO_EDGE(true) or REPEAT(false)
    bool SetTextureWrapping(const std::string& name, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR);

    RenderObject(MODE_TYPE t);
    ~RenderObject();
    
private:
    class Impl;
    Impl* m_imp;
};

#endif //_RENDEROBJECT_H_
