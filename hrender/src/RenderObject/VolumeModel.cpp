#include <string>
#include "VolumeModel.h"

class VolumeModel::Impl
{
public:
    Impl()
    {
        
    }
    ~Impl()
    {
        
    }

    bool SetShader(const std::string& shaderfile)
    {
        m_shaderfile = shaderfile;
        return true;
    }
    
    bool Create(BufferVolumeData* volume)
    {
        m_volume   = volume;
        return true;
    }
    
    BufferVolumeData* GetVolume() const {
        return m_volume;
    }
    
    const std::string& GetShader() const
    {
        return m_shaderfile;
    }

private:
    BufferVolumeData* m_volume;
    std::string m_shaderfile;

};
VolumeModel::VolumeModel() : RenderObject(TYPE_VOLUME)
{
    m_imp = new Impl();
}

VolumeModel::~VolumeModel()
{
    delete m_imp;
}

bool VolumeModel::SetShader(const std::string& shaderfile)
{
    return m_imp->SetShader(shaderfile);
}

bool VolumeModel::Create(BufferVolumeData* volume)
{
    return m_imp->Create(volume);
}

BufferVolumeData* VolumeModel::GetVolume() const
{
    return m_imp->GetVolume();
}

const std::string& VolumeModel::GetShader() const
{
    return m_imp->GetShader();
}

