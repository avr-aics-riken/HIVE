/**
 * @file SparseVolumeModel.cpp
 * ボリュームモデル
 */
#include <string>
#include "SparseVolumeModel.h"

/**
 * ボリュームモデル
 */
class SparseVolumeModel::Impl
{
public:
    /// コンストラクタ
    Impl()
    : m_clampToEdgeS(false)
    , m_clampToEdgeT(false)
    , m_clampToEdgeR(false)
    {
        
    }
    /// デストラクタ
    ~Impl()
    {
        
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
    bool Create(BufferSparseVolumeData* volume)
    {
        if (!volume || volume->GetType() != BufferData::TYPE_SPARSEVOLUME) {
			return false;
		}
        m_volume = volume;
        return true;
    }
    
    /// ボリュームデータの取得.
    BufferSparseVolumeData* GetSparseVolume() const {
        return m_volume;
    }
    
    /// シェーダパスを返す.
    const std::string& GetShader() const
    {
        return m_shaderfile;
    }

private:
    RefPtr<BufferSparseVolumeData> m_volume;
    std::string m_shaderfile;
    bool        m_clampToEdgeS;
    bool        m_clampToEdgeT;
    bool        m_clampToEdgeR;

};
/// コンストラクタ
SparseVolumeModel::SparseVolumeModel() : RenderObject(TYPE_SPARSEVOLUME)
{
    m_imp = new Impl();
}

/// デストラクタ
SparseVolumeModel::~SparseVolumeModel()
{
    delete m_imp;
}

/**
 * シェーダパスの設定
 * @param shaderfile シェーダパス
 */
bool SparseVolumeModel::SetShader(const std::string& shaderfile)
{
    return m_imp->SetShader(shaderfile);
}

/**
 * Set wrapping mode
 * @param s wrap mode for coord S
 * @param t wrap mode for coord T
 * @param r wrap mode for coord R
 */
bool SparseVolumeModel::SetClampToEdge(bool s, bool t, bool r)
{
    return m_imp->SetClampToEdge(s, t, r);
}

bool SparseVolumeModel::GetClampToEdgeS() const
{
    return m_imp->GetClampToEdgeS();
}

bool SparseVolumeModel::GetClampToEdgeT() const
{
    return m_imp->GetClampToEdgeT();
}

bool SparseVolumeModel::GetClampToEdgeR() const
{
    return m_imp->GetClampToEdgeR();
}

/**
 * ボリュームデータの生成
 * @retval true 成功
 * @retval false 失敗
 */
bool SparseVolumeModel::Create(BufferSparseVolumeData* volume)
{
    return m_imp->Create(volume);
}

/// ボリュームデータの取得.
BufferSparseVolumeData* SparseVolumeModel::GetSparseVolume() const
{
    return m_imp->GetSparseVolume();
}

/// シェーダパスを返す.
const std::string& SparseVolumeModel::GetShader() const
{
    return m_imp->GetShader();
}

