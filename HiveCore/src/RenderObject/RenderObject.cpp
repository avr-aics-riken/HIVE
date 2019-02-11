/**
 * @file RenderObject.cpp
 * レンダーオブジェクト
 */
#include <limits>
#include "RenderObject.h"
#include "../Buffer/BufferImageData.h"

#include "BufferExtraData.h"
#include "Buffer.h"


/**
 * レンダーオブジェクト
 */
class RenderObject::Impl
{
public:
    /// モードタイプを返す.
    RenderObject::MODE_TYPE GetType() const {
        return m_type;
    }

    /// タイプ文字列を返す.
    std::string GetTypeString() const {
        static const char* modeString[] = {
            "NONE",
            "SETTING",
            "CAMERA",
            "VOLUME",
            "SPARSEVOLUME",
            "POLYGON",
            "LINE",
            "POINT",
            "TETRA",
            "SOLID",
            "VECTOR"
        };
        return std::string(modeString[m_type]);
    }

    //--------------------------------------------------
    //Get
    //--------------------------------------------------
    /// 移動量を返す
    const VX::Math::vec3&  GetTranslate() const { return m_trans; }
    /// 回転量を返す
    const VX::Math::vec3&  GetRotate()    const { return m_rotat; }
    /// スケールを返す
    const VX::Math::vec3&  GetScale()     const { return m_scale; }
    /// 変換行列を返す
    const VX::Math::matrix GetTransformMatrix() const
    {
        return m_mat;
    }

    /// 変換行列を計算する
    const VX::Math::matrix calcMatrix() const {
        return VX::Math::Translation(m_trans[0], m_trans[1], m_trans[2])
         * VX::Math::RotationYawPitchRoll(m_rotat[1], m_rotat[0], m_rotat[2])
         * VX::Math::Scaling(m_scale[0], m_scale[1], m_scale[2]); // Y,X,Z
    }

    typedef std::map<std::string, VX::Math::vec4> Vec4Map;
    typedef std::map<std::string, VX::Math::vec3> Vec3Map;
    typedef std::map<std::string, VX::Math::vec2> Vec2Map;
    typedef std::map<std::string, float>         FloatMap;
    typedef std::map<std::string, int>         IntMap;
    typedef std::map<std::string, RefPtr<const BufferImageData> > TextureMap;
    typedef std::map<std::string, bool>          FilteringParamMap;
    /// Uniformマップを返す
    const Vec4Map&    GetUniformVec4 ()   const { return m_vec4s;  }
    /// Uniformマップを返す
    const Vec3Map&    GetUniformVec3 ()   const { return m_vec3s;  }
    /// Uniformマップを返す
    const Vec2Map&    GetUniformVec2 ()   const { return m_vec2s;  }
    /// Uniformマップを返す
    const FloatMap&   GetUniformFloat()   const { return m_floats; }
    /// Uniformマップを返す
    const IntMap&   GetUniformInt()   const { return m_ints; }
    /// Uniformテクスチャマップを返す
    const TextureMap& GetUniformTexture() const { return m_imgs;   }
    /// Filtering parameterを返す
    const FilteringParamMap& GetTextureFiltering() const { return m_filteringParams;   }
    /// Wrappign parameterを返す
    const WrappingParamMap& GetTextureWrapping() const { return m_wrappingParams;   }

    //--------------------------------------------------
    //Set
    //--------------------------------------------------
    /**
     * 移動量の設定
     * @param x x
     * @param y y
     * @param z z
     */
    bool SetTranslate(float x, float y, float z) {
        m_trans[0] = x;
        m_trans[1] = y;
        m_trans[2] = z;
        m_mat = calcMatrix();
        return true;
    }

    /**
     * 回転量の設定
     * @param x x(radian)
     * @param y y(radian)
     * @param z z(radian)
     */
    bool SetRotate(float x, float y, float z) {
        m_rotat[0] = x;
        m_rotat[1] = y;
        m_rotat[2] = z;
        m_mat = calcMatrix();
        return true;
    }

    /**
     * スケールの設定
     * @param x x
     * @param y y
     * @param z z
     */
    bool SetScale(float x, float y, float z) {
        m_scale[0] = x;
        m_scale[1] = y;
        m_scale[2] = z;
        m_mat = calcMatrix();
        return true;
    }

    /**
     * スケールの設定
     * @param m matrix
     */
    bool SetTransformMatrix(const float* m) {
        for (int i = 0; i < 16; ++i) {
            m_mat.f[i] = m[i];
        }
        return true;
    }

    /**
     * Unifrom値の設定
     * @param name Uniform名
     * @param x x
     * @param y y
     * @param z z
     * @param w w
     */
    bool SetVec4(const std::string& name, float x, float y, float z, float w) {
        m_vec4s[name] = VX::Math::vec4(x,y,z,w);
        return true;
    }

    /**
     * Unifrom値の設定
     * @param name Uniform名
     * @param x x
     * @param y y
     * @param z z
     */
    bool SetVec3(const std::string& name, float x, float y, float z) {
        m_vec3s[name] = VX::Math::vec3(x,y,z);
        return true;
    }

    /**
     * Unifrom値の設定
     * @param name Uniform名
     * @param x x
     * @param y y
     */
    bool SetVec2(const std::string& name, float x, float y) {
        m_vec2s[name] = VX::Math::vec2(x,y);
        return true;
    }

    /**
     * Unifrom値の設定
     * @param name Uniform名
     * @param x x
     */
    bool SetFloat(const std::string& name, float x) {
        m_floats[name] = x;
        return true;
    }

    /**
     * Unifrom値の設定
     * @param name Uniform名
     * @param x x
     */
    bool SetInt(const std::string& name, float x) {
        m_ints[name] = x;
        return true;
    }
    
    /**
     * テクスチャの設定
     * @param name Uniform名
     * @param img イメージデータ
     */
    bool SetTexture(const std::string& name, const BufferImageData* img) {
		if (!img) { return false; }
        if (img->Bytes() <= 0) {
            return false;
        }
        m_imgs[name] = img;
        return true;
    }

    /**
     * テクスチャ filtering の設定
     * @param name Uniform名
     * @param filter filtering flag
     */
    bool SetTextureFiltering(const std::string& name, bool filter) {
        m_filteringParams[name] = filter;
        return true;
    }

    /**
     * テクスチャ wrapping の設定
     * @param name Uniform名
     * @param texture wrapping flag
     */
    bool SetTextureWrapping(const std::string& name, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR) {
        std::vector<bool> wrappings;
        wrappings.push_back(clampToEdgeS);
        wrappings.push_back(clampToEdgeT);
        wrappings.push_back(clampToEdgeR);
        m_wrappingParams[name] = wrappings;
        return true;
    }

    typedef std::map<std::string, RefPtr<BufferExtraData> >  ExtraBufferMap;

    /**
     * 拡張バッファの追加
     * @param varyingName varying名
     * @param data 拡張バッファ
     */
    bool AddExtraBuffer(const std::string& varyingName, BufferExtraData* data)
    {
        m_extrabuffers[varyingName] = data;
        return true;
    }

    /**
     * 拡張バッファの削除
     * @param varyingName varying名
     */
    bool RemoveExtraBuffer(const std::string& varyingName)
    {
        ExtraBufferMap::iterator it = m_extrabuffers.find(varyingName);
        if (it != m_extrabuffers.end()) {
            m_extrabuffers.erase(it);
            return true;
        }
        return false;
    }

    /**
     * 拡張バッファの削除
     * @param varyingName varying名
     */
    bool ClearExtraBuffer()
    {
        m_extrabuffers.clear();
        return true;
    }
    
    /**
     * 拡張バッファの取得
     */
    const ExtraBufferMap& GetExtraBuffers() const
    {
        return m_extrabuffers;
    }

    /// BBoxを計算する
    bool CalcBBoxFromVec3Buffer(VX::Math::vec3 &box_min,VX::Math::vec3 &box_max, const Vec3Buffer* positions) const
    {
        const float flt_max = std::numeric_limits<float>::max();
        VX::Math::vec3 vmin(flt_max, flt_max, flt_max);
        VX::Math::vec3 vmax(-flt_max, -flt_max, -flt_max);
        const float* buffer = positions->GetBuffer();
        for (int i = 0; i < positions->GetNum(); i++) {
            vmin[0] = (std::min)(vmin[0], buffer[i*3 + 0]);
            vmin[1] = (std::min)(vmin[1], buffer[i*3 + 1]);
            vmin[2] = (std::min)(vmin[2], buffer[i*3 + 2]);
            vmax[0] = (std::max)(vmax[0], buffer[i*3 + 0]);
            vmax[1] = (std::max)(vmax[1], buffer[i*3 + 1]);
            vmax[2] = (std::max)(vmax[2], buffer[i*3 + 2]);
        }
        if (vmin.x < vmax.x && vmin.y < vmax.y && vmin.z < vmax.z) {
            box_min = vmin;
            box_max = vmax;
            return true;
        }
        return false;
    }


    /// コンストラクタ
    Impl(MODE_TYPE t) : m_type(t)
    {
        m_trans =  VX::Math::vec3(0, 0, 0);
        m_rotat =  VX::Math::vec3(0, 0, 0);
        m_scale =  VX::Math::vec3(1, 1, 1);
        m_mat   =  VX::Math::Identity();
    };

    /// デストラクタ
    ~Impl() {};

private:
    // Transform↲
    VX::Math::vec3 m_trans;
    VX::Math::vec3 m_rotat;
    VX::Math::vec3 m_scale;
    VX::Math::matrix4x4 m_mat;

    //mapped data
    std::map<std::string, VX::Math::vec4> m_vec4s;
    std::map<std::string, VX::Math::vec3> m_vec3s;
    std::map<std::string, VX::Math::vec2> m_vec2s;
    std::map<std::string, float>          m_floats;
    std::map<std::string, int>            m_ints;
    std::map<std::string, RefPtr<const BufferImageData> > m_imgs;
    std::map<std::string, bool>           m_filteringParams;
    std::map<std::string, std::vector<bool> >  m_wrappingParams;

    //mode type
    RenderObject::MODE_TYPE m_type;


    // Extra buffer
    ExtraBufferMap m_extrabuffers;

};

/// コンストラクタ
RenderObject::RenderObject(MODE_TYPE t) : m_imp(new Impl(t)) {}
/// デストラクタ
RenderObject::~RenderObject() { delete m_imp; }

/// モードタイプを返す.
RenderObject::MODE_TYPE RenderObject::GetType() const { return m_imp->GetType();       }
/// タイプ文字列を返す.
std::string RenderObject::GetTypeString()       const { return m_imp->GetTypeString(); }

//--------------------------------------------------
//Get
//--------------------------------------------------
/// 移動量を返す
const VX::Math::vec3&  RenderObject::GetTranslate()               const { return m_imp->GetTranslate();       }
/// 回転量を返す
const VX::Math::vec3&  RenderObject::GetRotate()                  const { return m_imp->GetRotate();          }
/// スケールを返す
const VX::Math::vec3&  RenderObject::GetScale()                   const { return m_imp->GetScale();           }
/// 変換行列を返す
const VX::Math::matrix RenderObject::GetTransformMatrix()         const { return m_imp->GetTransformMatrix(); }
/// Uniformマップを返す
const RenderObject::Vec4Map&  RenderObject::GetUniformVec4 ()     const { return m_imp->GetUniformVec4();     }
/// Uniformマップを返す
const RenderObject::Vec3Map&  RenderObject::GetUniformVec3 ()     const { return m_imp->GetUniformVec3();     }
/// Uniformマップを返す
const RenderObject::Vec2Map&  RenderObject::GetUniformVec2 ()     const { return m_imp->GetUniformVec2();     }
/// Uniformマップを返す
const RenderObject::FloatMap& RenderObject::GetUniformFloat()     const { return m_imp->GetUniformFloat();    }
/// Uniformマップを返す
const RenderObject::IntMap& RenderObject::GetUniformInt()     const { return m_imp->GetUniformInt();    }
/// Uniformテクスチャマップを返す
const RenderObject::TextureMap& RenderObject::GetUniformTexture() const { return m_imp->GetUniformTexture();  }
/// Filtering parameterマップを返す
const RenderObject::FilteringParamMap& RenderObject::GetTextureFiltering() const { return m_imp->GetTextureFiltering();  }
/// Wrapping parameterマップを返す
const RenderObject::WrappingParamMap& RenderObject::GetTextureWrapping() const { return m_imp->GetTextureWrapping();  }

//--------------------------------------------------
//Set
//--------------------------------------------------
/**
 * 移動量の設定
 * @param x x
 * @param y y
 * @param z z
 */
bool RenderObject::SetTranslate(float x, float y, float z)                              { return m_imp->SetTranslate(x, y, z);     }
/**
 * 回転量の設定
 * @param x x(radian)
 * @param y y(radian)
 * @param z z(radian)
 */
bool RenderObject::SetRotate(float x, float y, float z)                                 { return m_imp->SetRotate(x, y, z);        }
/**
 * スケールの設定
 * @param x x
 * @param y y
 * @param z z
 */
bool RenderObject::SetScale(float x, float y, float z)                                  { return m_imp->SetScale(x, y, z);         }

/**
 * マトリックスの設定
 * @param m matrix
 */
bool RenderObject::SetTransformMatrix(const float* m)                                   { return m_imp->SetTransformMatrix(m);     }

/**
 * Unifrom値の設定
 * @param name Uniform名
 * @param x x
 * @param y y
 * @param z z
 * @param w w
 */
bool RenderObject::SetVec4(const std::string& name, float x, float y, float z, float w) { return m_imp->SetVec4(name, x, y, z, w); }
/**
 * Unifrom値の設定
 * @param name Uniform名
 * @param x x
 * @param y y
 * @param z z
 */
bool RenderObject::SetVec3(const std::string& name, float x, float y, float z)          { return m_imp->SetVec3(name, x, y, z);    }
/**
 * Unifrom値の設定
 * @param name Uniform名
 * @param x x
 * @param y y
 */
bool RenderObject::SetVec2(const std::string& name, float x, float y)                   { return m_imp->SetVec2(name, x, y);       }
/**
 * Unifrom値の設定
 * @param name Uniform名
 * @param x x
 */
bool RenderObject::SetFloat(const std::string& name, float x)                           { return m_imp->SetFloat(name, x);         }
/**
 * Unifrom値の設定
 * @param name Uniform名
 * @param x x
 */
bool RenderObject::SetInt(const std::string& name, int x)                           { return m_imp->SetInt(name, x);         }
/**
 * テクスチャの設定
 * @param name Uniform名
 * @param img イメージデータ
 */
bool RenderObject::SetTexture(const std::string& name, const BufferImageData* img)      { return m_imp->SetTexture(name, img);     }
/**
 * テクスチャ filtering の設定
 * @param name Uniform名
 * @param filter filter flag
 */
bool RenderObject::SetTextureFiltering(const std::string& name, bool filter)      { return m_imp->SetTextureFiltering(name, filter);     }
/**
 * テクスチャ wrapping の設定
 * @param name Uniform名
 * @param clampToEdgeS `true` to set wrapping mode for S to CLAMP_TO_EDGE
 * @param clampToEdgeT `true` to set wrapping mode for T to CLAMP_TO_EDGE
 * @param clampToEdgeR `true` to set wrapping mode for R to CLAMP_TO_EDGE
 */
bool RenderObject::SetTextureWrapping(const std::string& name, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR)      { return m_imp->SetTextureWrapping(name, clampToEdgeS, clampToEdgeT, clampToEdgeR);     }



/// 拡張バッファを追加する
bool RenderObject::AddExtraBuffer(const std::string& varyingName, BufferExtraData* data)
{
    return m_imp->AddExtraBuffer(varyingName, data);
}

/// 拡張バッファを削除する
bool RenderObject::RemoveExtraBuffer(const std::string& varyingName)
{
    return m_imp->RemoveExtraBuffer(varyingName);
}

/// 拡張バッファを取得する
const RenderObject::ExtraBufferMap& RenderObject::GetExtraBuffers() const
{
    return m_imp->GetExtraBuffers();
}

/// 拡張バッファを全て削除する
bool RenderObject::ClearExtraBuffer()
{
    return m_imp->ClearExtraBuffer();
}

/// BBoxを計算する
bool RenderObject::CalcBBoxFromVec3Buffer(VX::Math::vec3 &bmin,VX::Math::vec3 &bmax, const Vec3Buffer* positions) const
{
    return m_imp->CalcBBoxFromVec3Buffer(bmin, bmax, positions);
}

