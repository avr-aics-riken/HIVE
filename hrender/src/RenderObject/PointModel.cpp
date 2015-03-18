/**
 * @file PointModel.cpp
 * ポイントモデル
 */
#include <string>
#include "PointModel.h"
#include "BufferPointData.h"

/**
 * ポイントモデル
 */
class PointModel::Impl
{
public:
    /// コンストラクタ
    Impl()
    {
        m_point = 0;
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
     * ポイントデータの生成
     * @retval true 成功 
     * @retval false 失敗
     */
    bool Create(BufferPointData* pointdata)
    {
        if (!pointdata)
            return false;
        m_point = pointdata;
        return true;
    }
    
    /**
     * ポイントデータの取得.
     * @retval ポイントデータ
     */
    BufferPointData* GetPoint() const {
        return m_point;
    }
    
    /**
     * シェーダパスを返す.
     * @retval シェーダパス
     */
    const std::string& GetShader() const
    {
        return m_shaderfile;
    }

private:
    RefPtr<BufferPointData> m_point;
    std::string m_shaderfile;

};

/// コンストラクタ
PointModel::PointModel() : RenderObject(TYPE_POINT)
{
    m_imp = new Impl();
}

/// デストラクタ
PointModel::~PointModel()
{
    delete m_imp;
}

/**
 * シェーダパスの設定
 * @param shaderfile シェーダパス
 */
bool PointModel::SetShader(const std::string& shaderfile)
{
    return m_imp->SetShader(shaderfile);
}

/**
 * ポイントデータの生成
 * @retval true 成功
 * @retval false 失敗
 */
bool PointModel::Create(BufferPointData* pointdata)
{
    return m_imp->Create(pointdata);
}

/**
 * ポイントデータの取得.
 * @retval ポイントデータ
 */
BufferPointData* PointModel::GetPoint() const
{
    return m_imp->GetPoint();
}

/**
 * シェーダパスを返す.
 * @retval シェーダパス
 */
const std::string& PointModel::GetShader() const
{
    return m_imp->GetShader();
}

