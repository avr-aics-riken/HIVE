/**
 * @file PointModel.cpp
 * ポイントモデル
 */
#include <string>
#include "PointModel.h"
#include "BufferPointData.h"

#include <limits>
#include "Buffer.h"

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
        if (!pointdata || pointdata->GetType() != BufferData::TYPE_POINT) {
			return false;
		}
        m_point = pointdata;
        return true;
    }
    
    /**
     * ポイントデータの取得.
     * @return ポイントデータ
     */
    BufferPointData* GetPoint() const {
        return m_point;
    }
    
    /**
     * シェーダパスを返す.
     * @return シェーダパス
     */
    const std::string& GetShader() const
    {
        return m_shaderfile;
    }
    
    
    bool GetBox(VX::Math::vec3& box_min, VX::Math::vec3& box_max) const
    {
        if (!m_point) { return false; }
        const Vec3Buffer* position = m_point->Position();
        const FloatBuffer* rad = m_point->Radius();
        const float flt_max = std::numeric_limits<float>::max();
        VX::Math::vec3 vmin(flt_max, flt_max, flt_max);
        VX::Math::vec3 vmax(-flt_max, -flt_max, -flt_max);
        if (rad) {
            const float* radius = rad->GetBuffer();
            const float* buffer = position->GetBuffer();
            for (int i = 0; i < position->GetNum(); i++) {
                vmin[0] = (std::min)(vmin[0], buffer[i*3 + 0] - radius[i]);
                vmin[1] = (std::min)(vmin[1], buffer[i*3 + 1] - radius[i]);
                vmin[2] = (std::min)(vmin[2], buffer[i*3 + 2] - radius[i]);
                vmax[0] = (std::max)(vmax[0], buffer[i*3 + 0] + radius[i]);
                vmax[1] = (std::max)(vmax[1], buffer[i*3 + 1] + radius[i]);
                vmax[2] = (std::max)(vmax[2], buffer[i*3 + 2] + radius[i]);
            }
            if (vmin.x < vmax.x && vmin.y < vmax.y && vmin.z < vmax.z) {
                box_min = vmin;
                box_max = vmax;
                return true;
            }
        } else {
            const float* buffer = position->GetBuffer();
            for (int i = 0; i < position->GetNum(); i++) {
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
        }
        return false;
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
 * @return ポイントデータ
 */
BufferPointData* PointModel::GetPoint() const
{
    return m_imp->GetPoint();
}

/**
 * シェーダパスを返す.
 * @return シェーダパス
 */
const std::string& PointModel::GetShader() const
{
    return m_imp->GetShader();
}

/// BBoxを取得
bool PointModel::GetBox(VX::Math::vec3& box_min, VX::Math::vec3& box_max) const
{
    return m_imp->GetBox(box_min, box_max);
}
