/*
	 RenderObject.h
 */
#ifndef _RENDEROBJECT_H_
#define _RENDEROBJECT_H_

#include "../Core/Ref.h"

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

    MODE_TYPE GetType() const {
        return m_type;
    }
protected:
    RenderObject(MODE_TYPE t) : m_type(t) {};
    ~RenderObject() {};
private:
    MODE_TYPE       m_type;
};

#endif //_RENDEROBJECT_H_
