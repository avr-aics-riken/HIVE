#ifndef _VOLUMEMODEL_H_
#define _VOLUMEMODEL_H_

#include "RenderObject.h"
#include "BufferMeshData.h"

class VolumeModel : public RenderObject
{
public:
	VolumeModel();
    ~VolumeModel();
    bool SetShader(const std::string& shaderfile);
    //bool Create(BufferMeshData *mesh);
};

#endif //_VOLUMEMODEL_H_

