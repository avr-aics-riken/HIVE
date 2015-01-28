#include "VolumeModel.h"

VolumeModel::VolumeModel() : RenderObject(TYPE_VOLUME)
{
}

VolumeModel::~VolumeModel()
{
}

bool VolumeModel::SetShader(const std::string& shaderfile)
{
    m_shaderfile = shaderfile;
    return true;
}

bool VolumeModel::Create(BufferVolumeData* volume)
{
    m_volume   = volume;
    return true;
}

BufferVolumeData* VolumeModel::GetVolume() const {
    return m_volume;
}

const std::string& VolumeModel::GetShader() const
{
    return m_shaderfile;
}

