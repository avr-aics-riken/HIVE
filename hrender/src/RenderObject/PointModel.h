#ifndef _POINTMODEL_H_
#define _POINTMODEL_H_

#include "RenderObject.h"
#include "BufferMeshData.h"

class PointModel : public RenderObject
{
public:
	PointModel();
    ~PointModel();
    bool SetShader(const std::string& shaderfile);
    //bool Create(BufferMeshData *mesh);
};

#endif //_POINTMODEL_H_

