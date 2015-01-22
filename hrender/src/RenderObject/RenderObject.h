/*
    RenderObject.h
*/

class RenderObject : public LuaRef
{
public:
    RenderObject(){};
    ~RenderObject(){};
    
};

LUA_SCRIPTCLASS_CAST_AND_PUSH(RenderObject)