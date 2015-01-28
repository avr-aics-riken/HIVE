#ifndef _VOLUMEMODEL_H_
#define _VOLUMEMODEL_H_

#include "RenderObject.h"
#include "LuaUtil.h"

class BufferVolumeData;

class VolumeModel : public RenderObject
{
private:
    BufferVolumeData* m_volume;
    std::string m_shaderfile;
public:
    VolumeModel();
    ~VolumeModel();
    bool SetShader(const std::string& shaderfile);
    bool Create(BufferVolumeData* volume);
    BufferVolumeData* GetVolume() const;
    const std::string& GetShader() const;

};

#endif //_VOLUMEMODEL_H_

