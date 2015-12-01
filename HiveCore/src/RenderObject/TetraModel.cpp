/**
 * @file TetraModel.cpp
 * 三角錐モデル
 */
#include <string>
#include "TetraModel.h"
#include "BufferTetraData.h"

/**
 * 三角錐モデル
 */
class TetraModel::Impl
{
public:
    /// コンストラクタ
    Impl()
    {
        m_tetra = 0;
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
     * 三角錐データの生成
     * @retval true 成功
     * @retval false 失敗
     */
    bool Create(BufferTetraData* tetradata)
    {
        if (!tetradata || tetradata->GetType() != BufferData::TYPE_TETRA) {
			return false;
		}
        
        m_tetra = tetradata;
        
        return true;
    }
    
    /// 三角錐データの取得.
    BufferTetraData* GetTetra() const {
        return m_tetra;
    }
    
    /// シェーダパスを返す.
    const std::string& GetShader() const {
        return m_shaderfile;
    }

private:
    RefPtr<BufferTetraData> m_tetra;
    std::string m_shaderfile;
};

/// コンストラクタ
TetraModel::TetraModel() : RenderObject(TYPE_TETRA)
{
    m_imp = new Impl();
}

/// デストラクタ
TetraModel::~TetraModel()
{
    delete m_imp;
}

/**
 * シェーダパスの設定
 * @param shaderfile シェーダパス
 */
bool TetraModel::SetShader(const std::string& shaderfile)
{
    return m_imp->SetShader(shaderfile);
}

/**
 * 三角錐データの生成
 * @retval true 成功
 * @retval false 失敗
 */
bool TetraModel::Create(BufferTetraData* tetradata)
{
    return m_imp->Create(tetradata);
}

/// 三角錐データの取得.
BufferTetraData* TetraModel::GetTetra() const
{
    return m_imp->GetTetra();
}

/// シェーダパスを返す.
const std::string& TetraModel::GetShader() const
{
    return m_imp->GetShader();
}

