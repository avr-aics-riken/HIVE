#ifndef _VOLLOADER_LUA_H_
#define _VOLLOADER_LUA_H_

#include "LuaUtil.h"
#include "VolLoader.h"

class VOLLoader_Lua : public VOLLoader
{
public:
    VOLLoader_Lua();
    ~VOLLoader_Lua();
    void Clear();
    bool Load(const char* filename);
    int Width();
    int Height();
    int Depth();
    int Component();
    LUA_SCRIPTCLASS_BEGIN(VOLLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Width)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Height)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Depth)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Component)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool,Load,const char*)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(VOLLoader_Lua);

#endif //_VOLLOADER_LUA_H_

