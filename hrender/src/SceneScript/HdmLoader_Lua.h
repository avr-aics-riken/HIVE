/**
 * @file HDMLoader_Lua.h
 * HDMLoader Luaラッパー
 */
#ifndef _HDMLOADER_LUA_H_
#define _HDMLOADER_LUA_H_

#include "LuaUtil.h"
#include "HdmLoader.h"
#include "BufferSparseVolumeData_Lua.h"
/**
 * HDMLoader Luaラッパー
 */
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

    bool Load(const char* cellidFilename, const char* dataFilename, const char* fieldName, const char* fieldType, int components, int virtualCells=2) {
        return HDMLoader::Load(cellidFilename, dataFilename, fieldName, fieldType, components, virtualCells);
    }

    BufferSparseVolumeData_Lua* SparseVolumeData() {
        return new BufferSparseVolumeData_Lua(HDMLoader::SparseVolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(HDMLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Width)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Height)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Depth)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,Component)
    LUA_SCRIPTCLASS_METHOD_ARG6(bool,Load,const char*, const char*, const char*, const char*, int, int)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferSparseVolumeData_Lua*,SparseVolumeData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(HDMLoader_Lua);

#endif //_HDMLOADER_LUA_H_

