/**
 * @file PolygonModel.cpp
 * ポリゴンモデル
 */
#include <string>
#include <stdio.h>
#include "PolygonModel.h"
#include "BufferMeshData.h"


// TEST for ExtraBuffer
//#include "Buffer.h"
//#include "BufferExtraData.h"

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
        if (!m || m->GetType() != BufferData::TYPE_MESH) {
            return false;
        }
        m_mesh = m;
        
        return true;
    }
    
    /**
     * メッシュの取得.
     * @return メッシュデータ
     */
    BufferMeshData* GetMesh() const {
        return m_mesh;
    }
    
    /**
     * シェーダパスを返す.
     * @return シェーダパス
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
    /*
    // TEST for ExtraBuffer
    const int vnum = m->Position()->GetNum();
    BufferExtraData* ebuf = new BufferExtraData();
    ebuf->Create("vec3", vnum);
    for (int i = 0; i < vnum; ++i) {
        ebuf->Vec3Buffer()->GetBuffer()[3*i  ] = 1.0 - i/static_cast<float>(vnum);
        ebuf->Vec3Buffer()->GetBuffer()[3*i+1] = 0.0;
        ebuf->Vec3Buffer()->GetBuffer()[3*i+2] = 0.0;
    }
    AddExtraBuffer(std::string("testvec3"), ebuf);
    */
    
    return m_imp->Create(m);
}

/**
 * メッシュの取得.
 * @return メッシュデータ
 */
BufferMeshData* PolygonModel::GetMesh() const
{
    return m_imp->GetMesh();
}

/**
 * シェーダパスを返す.
 * @return シェーダパス
 */
const std::string& PolygonModel::GetShader() const
{
    return m_imp->GetShader();
}

/// BBoxを取得
bool PolygonModel::GetBox(VX::Math::vec3& box_min, VX::Math::vec3& box_max) const
{
    if (GetMesh()) {
        return RenderObject::CalcBBoxFromVec3Buffer(box_min, box_max, GetMesh()->Position());
    }
    return false;
}
