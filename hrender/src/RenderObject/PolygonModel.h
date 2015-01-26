#ifndef _POLYGONMODEL_H_
#define _POLYGONMODEL_H_

#include "RenderObject.h"
#include "LuaUtil.h"
#include "BufferMeshData.h"

class PolygonModel : public RenderObject
{
public:
	PolygonModel();
    ~PolygonModel();
    bool SetShader(const std::string& shaderfile);
    bool Create(BufferMeshData &mesh);
};

#endif //_POLYGONMODEL_H_

