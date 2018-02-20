/**
 * @file SolidModel.cpp
 * Solid モデル
 */
#include <string>
#include "SolidModel.h"
#include "BufferSolidData.h"

/**
 * Solidモデル
 */
class SolidModel::Impl
{
public:
    /// コンストラクタ
    Impl()
    {
        m_solid = 0;
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
     * Solidデータの生成
     * @retval true 成功
     * @retval false 失敗
     */
    bool Create(BufferSolidData* soliddata)
    {
        if (!soliddata || soliddata->GetType() != BufferData::TYPE_SOLID) {
			return false;
		}
        
        m_solid = soliddata;
        
        return true;
    }
    
    /// Solidデータの取得.
    BufferSolidData* GetSolid() const {
        return m_solid;
    }
    
    /// シェーダパスを返す.
    const std::string& GetShader() const {
        return m_shaderfile;
    }

private:
    RefPtr<BufferSolidData> m_solid;
    std::string m_shaderfile;
};

/// コンストラクタ
SolidModel::SolidModel() : RenderObject(TYPE_SOLID)
{
    m_imp = new Impl();
}

/// デストラクタ
SolidModel::~SolidModel()
{
    delete m_imp;
}

/**
 * シェーダパスの設定
 * @param shaderfile シェーダパス
 */
bool SolidModel::SetShader(const std::string& shaderfile)
{
    return m_imp->SetShader(shaderfile);
}

/**
 * Solidデータの生成
 * @retval true 成功
 * @retval false 失敗
 */
bool SolidModel::Create(BufferSolidData* soliddata)
{
    return m_imp->Create(soliddata);
}

/// Solidデータの取得.
BufferSolidData* SolidModel::GetSolid() const
{
    return m_imp->GetSolid();
}

/// シェーダパスを返す.
const std::string& SolidModel::GetShader() const
{
    return m_imp->GetShader();
}

/// BBoxを取得
bool SolidModel::GetBox(VX::Math::vec3& box_min, VX::Math::vec3& box_max) const
{
    if (GetSolid()) {
        return RenderObject::CalcBBoxFromVec3Buffer(box_min, box_max, GetSolid()->Position());
    }
    return false;
}

