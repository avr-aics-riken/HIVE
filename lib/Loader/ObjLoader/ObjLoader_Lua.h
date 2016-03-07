/**
 * @file OBJLoader_Lua.h
 * OBJLoader Luaラッパー
 */
#ifndef _OBJLOADER_LUA_H_
#define _OBJLOADER_LUA_H_

#include "LuaUtil.h"
#include "Buffer.h"
#include "BufferMeshData_Lua.h"
#include "BufferPointData_Lua.h"
#include "BufferLineData_Lua.h"
#include "BufferTetraData_Lua.h"
#include "BufferVectorData_Lua.h"
 #include "BufferExtraData_Lua.h"
#include "ObjLoader.h"

#ifdef HIVE_WITH_PMLIB
#include "../Core/Perf.h"
#endif

/**
 * OBJLoader Luaラッパー
 */
class OBJLoader_Lua : public OBJLoader
{
public:
    OBJLoader_Lua(){}
    ~OBJLoader_Lua(){};

    bool Load(const char* filename){
#ifdef HIVE_WITH_PMLIB
        GetPM().start(HIVE_PERF_LABEL_LOADER_OBJLOAD);
#endif
        bool ret = OBJLoader::Load(filename);
#ifdef HIVE_WITH_PMLIB
        GetPM().stop(HIVE_PERF_LABEL_LOADER_OBJLOAD);
#endif
        return ret;
    }

    BufferMeshData_Lua* MeshData() {
        return BufferMeshData_Lua::CreateInstance(OBJLoader::MeshData());
    }

    BufferPointData_Lua* PointData() {
        return BufferPointData_Lua::CreateInstance(OBJLoader::PointData());
    }

    BufferLineData_Lua* LineData() {
        return BufferLineData_Lua::CreateInstance(OBJLoader::LineData());
    }

    BufferTetraData_Lua* TetraData() {
        return BufferTetraData_Lua::CreateInstance(OBJLoader::TetraData());
    }
    
    BufferVectorData_Lua* NormalData() {
        return BufferVectorData_Lua::CreateInstance(OBJLoader::NormalData());
    }

    // TEST
    BufferExtraData_Lua* ExtraData() {
        return BufferExtraData_Lua::CreateInstance(OBJLoader::ExtraData());
    }
    
    LUA_SCRIPTCLASS_BEGIN(OBJLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool,Load,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferMeshData_Lua*,   MeshData)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferPointData_Lua*,  PointData)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferLineData_Lua*,   LineData)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferTetraData_Lua*,  TetraData)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVectorData_Lua*, NormalData)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferExtraData_Lua*,  ExtraData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(OBJLoader_Lua);

#endif //_OBJLOADER_LUA_H_

