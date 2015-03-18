/**
 * @file VolumeModel.h
 * ボリュームモデル
 */
#ifndef _VOLUMEMODEL_H_
#define _VOLUMEMODEL_H_

#include <string>
#include "RenderObject.h"
#include "BufferVolumeData.h"

/**
 * ボリュームモデル
 */
class BufferVolumeData;

class VolumeModel : public RenderObject
{
private:
    class Impl;
    Impl* m_imp;
public:
    VolumeModel();
    ~VolumeModel();
    bool SetShader(const std::string& shaderfile);
    bool Create(BufferVolumeData* volume);
    BufferVolumeData* GetVolume() const;
    const std::string& GetShader() const;

};

#endif //_VOLUMEMODEL_H_

