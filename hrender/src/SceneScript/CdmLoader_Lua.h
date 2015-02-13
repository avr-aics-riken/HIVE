#ifndef _CDMLOADER_LUA_H_
#define _CDMLOADER_LUA_H_

#include "LuaUtil.h"
#include "CdmLoader.h"
#include "BufferVolumeData_Lua.h"

class CDMLoader_Lua : public CDMLoader
{
public:
    CDMLoader_Lua() {}
    ~CDMLoader_Lua() {}

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

    bool Load(const char* filename) {
        return CDMLoader::Load(filename);
    }

    BufferVolumeData_Lua* VolumeData() {
        return new BufferVolumeData_Lua(CDMLoader::VolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(CDMLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Width)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Height)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Depth)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Component)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool,Load,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(CDMLoader_Lua);

#endif //_CDMLOADER_LUA_H_

