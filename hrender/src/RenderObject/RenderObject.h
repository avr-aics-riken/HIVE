/*
	 RenderObject.h
 */
#ifndef _RENDEROBJECT_H_
#define _RENDEROBJECT_H_


#include "../Core/Ref.h"
#include "BufferMeshData.h"

class RenderObject : public RefCount
{
public:
	enum MODE_TYPE {
		TYPE_NONE,
		TYPE_SETTING,
		TYPE_CAMERA,
		TYPE_VOLUME,
		TYPE_POLYGON,
		TYPE_POINT,
		TYPE_MAX,
	};

	RenderObject(){};
	~RenderObject(){};
	MODE_TYPE GetType()             { return type; }
	BufferMeshData *Mesh()          { return mesh; }
	void SetType(MODE_TYPE t)       { type = t; }
	void SetMesh(BufferMeshData *m) { mesh = m; }
private:
	MODE_TYPE       type;
	BufferMeshData *mesh;
protected:
};

#endif //_RENDEROBJECT_H_
