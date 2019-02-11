/**
 * @file VolumeModel.cpp
 * ボリュームモデル
 */
#include <string>
#include "VolumeModel.h"

/**
 * ボリュームモデル
 */
class VolumeModel::Impl
{
public:
    /// コンストラクタ
    Impl()
    : m_clampToEdgeS(false)
    , m_clampToEdgeT(false)
    , m_clampToEdgeR(false)
    {
        m_volume = 0;
    }
    /// デストラクタ
    ~Impl()
    {
        m_volume = 0;
    }
    
    /**
     * シェーダパスの設定
     * @param shaderfile シェーダパス
     */
    bool SetShader(const std::string& shaderfile)
    {
        m_shaderfile = shaderfile;
        return true;
    }

    /**
     * Set wrapping mode
     * @param s clampToEdge for coord S
     * @param t clampToEdge for coord T
     * @param r clampToEdge for coord R
     */
    bool SetClampToEdge(bool s, bool t, bool r)
    {
        m_clampToEdgeS = s;
        m_clampToEdgeT = t;
        m_clampToEdgeR = r;
        return true;
    }
    
    bool GetClampToEdgeS() const {
        return m_clampToEdgeS;
    }

    bool GetClampToEdgeT() const {
        return m_clampToEdgeT;
    }

    bool GetClampToEdgeR() const {
        return m_clampToEdgeR;
    }
    
    
    /**
     * ボリュームデータの生成
     * @retval true 成功
     * @retval false 失敗
     */
    bool Create(BufferVolumeData* volume)
    {
        if (!volume || volume->GetType() != BufferData::TYPE_VOLUME) {
			return false;
		}
        m_volume = volume;
        return true;
    }
    
    /// ボリュームデータの取得.
    BufferVolumeData* GetVolume() const {
        return m_volume;
    }
    
    /// シェーダパスを返す.
    const std::string& GetShader() const
    {
        return m_shaderfile;
    }
    
    bool GetBox(VX::Math::vec3& box_min, VX::Math::vec3& box_max) const
    {
        if (m_volume) {
            box_min.x = - m_volume->Width() / 2.0f;
            box_min.y = - m_volume->Height() / 2.0f;
            box_min.z = - m_volume->Depth() / 2.0f;
            box_max = -box_min;
            if (!(box_max.x == 0.0 && box_max.y == 0.0 && box_max.z == 0.0)) {
                return true;
            }
        }
        return false;
    }

private:
    RefPtr<BufferVolumeData> m_volume;
    std::string m_shaderfile;
    bool        m_clampToEdgeS;
    bool        m_clampToEdgeT;
    bool        m_clampToEdgeR;

};
/// コンストラクタ
VolumeModel::VolumeModel() : RenderObject(TYPE_VOLUME)
{
    m_imp = new Impl();
}

/// デストラクタ
VolumeModel::~VolumeModel()
{
    delete m_imp;
}

/**
 * シェーダパスの設定
 * @param shaderfile シェーダパス
 */
bool VolumeModel::SetShader(const std::string& shaderfile)
{
    return m_imp->SetShader(shaderfile);
}

/**
 * Set wrapping mode
 * @param s wrap mode for coord S
 * @param t wrap mode for coord T
 * @param r wrap mode for coord R
 */
bool VolumeModel::SetClampToEdge(bool s, bool t, bool r)
{
    return m_imp->SetClampToEdge(s, t, r);
}

bool VolumeModel::GetClampToEdgeS() const
{
    return m_imp->GetClampToEdgeS();
}

bool VolumeModel::GetClampToEdgeT() const
{
    return m_imp->GetClampToEdgeT();
}

bool VolumeModel::GetClampToEdgeR() const
{
    return m_imp->GetClampToEdgeR();
}

/**
 * ボリュームデータの生成
 * @retval true 成功
 * @retval false 失敗
 */
bool VolumeModel::Create(BufferVolumeData* volume)
{
    return m_imp->Create(volume);
}

/// ボリュームデータの取得.
BufferVolumeData* VolumeModel::GetVolume() const
{
    return m_imp->GetVolume();
}

/// シェーダパスを返す.
const std::string& VolumeModel::GetShader() const
{
    return m_imp->GetShader();
}

/// BBoxの取得
bool VolumeModel::GetBox(VX::Math::vec3& bmin, VX::Math::vec3& bmax) const
{
    return m_imp->GetBox(bmin, bmax);
}
