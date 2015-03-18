/**
 * @file VolumeModel.cpp
 * ボリュームモデル
 */
#include <string>
#include "VolumeModel.h"

class VolumeModel::Impl
{
public:
    /// コンストラクタ
    Impl()
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
     * ボリュームデータの生成
     * @retval true 成功
     * @retval false 失敗
     */
    bool Create(BufferVolumeData* volume)
    {
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

private:
    RefPtr<BufferVolumeData> m_volume;
    std::string m_shaderfile;

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

