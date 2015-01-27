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

    MODE_TYPE GetType() const       { return m_type; }
    BufferMeshData* Mesh() const    { return m_mesh; }

protected:
    RenderObject(MODE_TYPE t) : m_type(t) {};
	~RenderObject(){};
    
	void SetMesh(BufferMeshData *m) { m_mesh = m;    }
    
private:
	MODE_TYPE       m_type;
	BufferMeshData* m_mesh;
    
protected:
};

#endif //_RENDEROBJECT_H_
