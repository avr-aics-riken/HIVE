#include "VolumeModel_Lua.h"

VolumeModel_Lua::VolumeModel_Lua()
{
}

VolumeModel_Lua::~VolumeModel_Lua()
{
}

bool VolumeModel_Lua::SetShader(const std::string& shaderfile)
{
    return VolumeModel::SetShader(shaderfile);
}

bool VolumeModel_Lua::Create(BufferVolumeData_Lua *mesh)
{
    VolumeModel::Create(mesh);
    return true;
}

