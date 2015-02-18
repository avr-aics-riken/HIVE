#ifndef _HDMLOADER_LUA_H_
#define _HDMLOADER_LUA_H_

#include "LuaUtil.h"
#include "HdmLoader.h"
#include "BufferVolumeData_Lua.h"

class HDMLoader_Lua : public HDMLoader
{
public:
    HDMLoader_Lua() {}
    ~HDMLoader_Lua() {}

    int Width() {
        return Width();
    }

    int Height() {
        return Height();
    }

    int Depth() {
        return Depth();
    }

    int Component() {
        return Component();
    }

    bool Load(const char* cellidFilename, const char* dataFilename) {
        return HDMLoader::Load(cellidFilename, dataFilename);
    }

    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(HDMLoader::VolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(HDMLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Width)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Height)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Depth)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Component)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool,Load,const char*, const char*)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(HDMLoader_Lua);

#endif //_HDMLOADER_LUA_H_
