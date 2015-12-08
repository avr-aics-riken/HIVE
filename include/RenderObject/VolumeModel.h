/**
 * @file VolumeModel.h
 * ボリュームモデル
 */
#ifndef _VOLUMEMODEL_H_
#define _VOLUMEMODEL_H_

#include <string>
#include "RenderObject.h"
#include "BufferVolumeData.h"


class BufferVolumeData;

/**
 * ボリュームモデル
 */
class VolumeModel : public RenderObject
{
private:
    class Impl;
    Impl* m_imp;
public:
    VolumeModel();
    ~VolumeModel();
    bool SetShader(const std::string& shaderfile);
    bool SetClampToEdge(bool s, bool t, bool r);
    bool Create(BufferVolumeData* volume);
    BufferVolumeData* GetVolume() const;
    const std::string& GetShader() const;

    bool GetClampToEdgeS() const;  
    bool GetClampToEdgeT() const; 
    bool GetClampToEdgeR() const; 

};

#endif //_VOLUMEMODEL_H_

