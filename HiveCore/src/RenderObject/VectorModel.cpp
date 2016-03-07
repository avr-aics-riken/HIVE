/**
 * @file VectorModel.cpp
 * ベクトルモデル
 */
#include <string>
#include "VectorModel.h"
#include "BufferVectorData.h"

/**
 * ベクトルモデル
 */
class VectorModel::Impl
{
public:
    /// コンストラクタ
    Impl()
    {
        m_vector      = 0;
        m_linewidth   = 0.5f;
        m_arrowsize   = 1.0f;
        m_lengthscale = 1.0f;
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
     * ベクトルデータの生成
     * @retval true 成功
     * @retval false 失敗
     */
    bool Create(BufferVectorData* vectordata)
    {
        if (!vectordata || vectordata->GetType() != BufferData::TYPE_VECTOR) {
			return false;
		}
        m_vector = vectordata;
        
        return true;
    }
    
    /// ベクトルデータの取得.
    BufferVectorData* GetVector() const {
        return m_vector;
    }
    
    /// シェーダパスを返す.
    const std::string& GetShader() const {
        return m_shaderfile;
    }
    
    /// ライン幅の取得.
    float GetLineWidth() const {
        return m_linewidth;
    }
    /**
     * ライン幅の設定
     * @param w ライン幅
     */
    void SetLineWidth(float w) {
        m_linewidth = w;
    }
    /// アローサイズの取得.
    float GetArrowSize() const {
        return m_arrowsize;
    }
    /**
     * アローサイズの設定.
     * @param s アローサイズ
     */
    void SetArrowSize(float s) {
        m_arrowsize = s;
    }
    /// 長さスケールの取得.
    float GetLengthScale() const {
        return m_lengthscale;
    }
    /**
     * 長さスケールの設定
     * @param s 長さスケール
     */
    void SetLengthScale(float s) {
        m_lengthscale = s;
    }

private:
    RefPtr<BufferVectorData>  m_vector;
    float m_linewidth;
    float m_arrowsize;
    float m_lengthscale;
    std::string m_shaderfile;
};

/// コンストラクタ
VectorModel::VectorModel() : RenderObject(TYPE_VECTOR) 
{
    m_imp = new Impl();
}
/// デストラクタ
VectorModel::~VectorModel()
{
    delete m_imp;
}

/**
 * シェーダパスの設定
 * @param shaderfile シェーダパス
 */
bool VectorModel::SetShader(const std::string& shaderfile)
{
    return m_imp->SetShader(shaderfile);
}

/**
 * ベクトルデータの生成
 * @retval true 成功
 * @retval false 失敗
 */
bool VectorModel::Create(BufferVectorData* vectordata)
{
    return m_imp->Create(vectordata);
}

/// ベクトルデータの取得.
BufferVectorData* VectorModel::GetVector()
{
    return m_imp->GetVector();
}

/// ベクトルデータの取得.
const BufferVectorData* VectorModel::GetVector() const
{
    return m_imp->GetVector();
}

/// シェーダパスを返す.
const std::string& VectorModel::GetShader() const
{
    return m_imp->GetShader();
}

/// ライン幅の取得.
float VectorModel::GetLineWidth() const {
    return m_imp->GetLineWidth();
}

/**
 * ライン幅の設定
 * @param w ライン幅
 */
void VectorModel::SetLineWidth(float s) {
    m_imp->SetLineWidth(s);
}

/// アローサイズの取得.
float VectorModel::GetArrowSize() const {
    return m_imp->GetArrowSize();
}

/**
 * アローサイズの設定.
 * @param s アローサイズ
 */
void VectorModel::SetArrowSize(float s) {
    m_imp->SetArrowSize(s);
}

/// 長さスケールの取得.
float VectorModel::GetLengthScale() const {
    return m_imp->GetLengthScale();
}

/**
 * 長さスケールの設定
 * @param s 長さスケール
 */
void VectorModel::SetLengthScale(float s) {
    m_imp->SetLengthScale(s);
}

