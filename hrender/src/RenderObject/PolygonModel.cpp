/**
 * @file PolygonModel.cpp
 * ポリゴンモデル
 */
#include <string>
#include "PolygonModel.h"
#include "BufferMeshData.h"

/**
 * ポリゴンモデル
 */
class PolygonModel::Impl
{
public:
    /// コンストラクタ
    Impl()
    {
        m_mesh = 0;
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
     * メッシュデータの生成
     * @retval true 成功
     * @retval false 失敗
     */
    bool Create(BufferMeshData* m)
    {
        if (!m) {
            return false;
        }
        m_mesh = m;
        return true;
    }
    
    /**
     * メッシュの取得.
     * @retval メッシュデータ
     */
    BufferMeshData* GetMesh() const {
        return m_mesh;
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
    RefPtr<BufferMeshData> m_mesh;
    std::string m_shaderfile;
};

/// コンストラクタ
PolygonModel::PolygonModel() : RenderObject(TYPE_POLYGON)
{
    m_imp = new Impl();
}

/// デストラクタ
PolygonModel::~PolygonModel()
{
    delete m_imp;
}

/**
 * シェーダパスの設定
 * @param shaderfile シェーダパス
 */
bool PolygonModel::SetShader(const std::string& shaderfile)
{
    return m_imp->SetShader(shaderfile);
}

/**
 * メッシュデータの生成
 * @retval true 成功
 * @retval false 失敗
 */
bool PolygonModel::Create(BufferMeshData* m)
{
    return m_imp->Create(m);
}

/**
 * メッシュの取得.
 * @retval メッシュデータ
 */
BufferMeshData* PolygonModel::GetMesh() const
{
    return m_imp->GetMesh();
}

/**
 * シェーダパスを返す.
 * @retval シェーダパス
 */
const std::string& PolygonModel::GetShader() const
{
    return m_imp->GetShader();
}
