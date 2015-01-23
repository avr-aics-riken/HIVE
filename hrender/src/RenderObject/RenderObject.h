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
        TYPE_CAMERA,
        TYPE_POLYGON,
        TYPE_VOLUME,
        TYPE_POINT,
    };
    
    RenderObject(){  };
    ~RenderObject(){};
    
    MODE_TYPE GetType() { return TYPE_POLYGON; };
};

#endif //_RENDEROBJECT_H_