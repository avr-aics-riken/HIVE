/**
 * @file RawSparseVolumeLoader_Lua.h
 * RawSparseVolumeLoader_Lua Luaラッパー
 */
#ifndef _RAWSPARSEVOLUMELOADER_LUA_H_
#define _RAWSPARSEVOLUMELOADER_LUA_H_

#include "LuaUtil.h"
#include "RawSparseVolumeLoader.h"
#include "BufferSparseVolumeData_Lua.h"
/**
 * RawSparseVolumeLoader_Lua Luaラッパー
 */
class RawSparseVolumeLoader_Lua : public RawSparseVolumeLoader
{
public:
    RawSparseVolumeLoader_Lua() {}
    ~RawSparseVolumeLoader_Lua() {}

    void Clear() {
        RawSparseVolumeLoader::Clear();
    }

    //bool Load(const char* filename, int width, int height, int depth, int components, const char* format) {
    //    return RawSparseVolumeLoader::Load(filename, width, height, depth, components, format);
    //}

    //int Width() {
    //    return RawSparseVolumeLoader::Width();
    //}

    //int Height() {
    //    return RawSparseVolumeLoader::Height();
    //}

    //int Depth() {
    //    return RawSparseVolumeLoader::Depth();
    //}

    //int Component() {
    //    return RawSparseVolumeLoader::Component();
    //}

    BufferSparseVolumeData_Lua* SparseVolumeData() {
        return new BufferSparseVolumeData_Lua(RawSparseVolumeLoader::SparseVolumeData());
    }

    LUA_SCRIPTCLASS_BEGIN(RawSparseVolumeLoader_Lua)
    //LUA_SCRIPTCLASS_METHOD_ARG0(int,Width)
    //LUA_SCRIPTCLASS_METHOD_ARG0(int,Height)
    //LUA_SCRIPTCLASS_METHOD_ARG0(int,Depth)
    //LUA_SCRIPTCLASS_METHOD_ARG0(int,Component)
    //LUA_SCRIPTCLASS_METHOD_ARG6(bool,Load,const char*,int,int,int,int,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool,LoadFromPointer, void*)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferSparseVolumeData_Lua*,SparseVolumeData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(RawSparseVolumeLoader_Lua);

#endif //_RAWSPARSEVOLUMELOADER_LUA_H_
