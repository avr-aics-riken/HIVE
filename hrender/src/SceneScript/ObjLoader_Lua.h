#ifndef _OBJLOADER_LUA_H_
#define _OBJLOADER_LUA_H_

#include "LuaUtil.h"
#include "Buffer.h"
#include "BufferMeshData_Lua.h"
#include "BufferPointData_Lua.h"
#include "BufferLineData_Lua.h"
#include "BufferTetraData_Lua.h"
#include "BufferVectorData_Lua.h"
#include "ObjLoader.h"

//------//------//------//------//------//------//------//------//------
//
//------//------//------//------//------//------//------//------//------
class OBJLoader_Lua : public OBJLoader
{
public:
    OBJLoader_Lua(){}
    ~OBJLoader_Lua(){};

    bool Load(const char* filename){
        return OBJLoader::Load(filename);
    }

    BufferMeshData_Lua* MeshData() {
        return new BufferMeshData_Lua(OBJLoader::MeshData());
    }

    BufferPointData_Lua* PointData() {
        return new BufferPointData_Lua(OBJLoader::PointData());
    }

    BufferLineData_Lua* LineData() {
        return new BufferLineData_Lua(OBJLoader::LineData());
    }

    BufferTetraData_Lua* TetraData() {
        return new BufferTetraData_Lua(OBJLoader::TetraData());
    }
    
    BufferVectorData_Lua* NormalData() {
        return new BufferVectorData_Lua(OBJLoader::NormalData());
    }
    
    LUA_SCRIPTCLASS_BEGIN(OBJLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool,Load,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferMeshData_Lua*,   MeshData)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferPointData_Lua*,  PointData)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferLineData_Lua*,   LineData)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferTetraData_Lua*,  TetraData)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVectorData_Lua*, NormalData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(OBJLoader_Lua);

#endif //_OBJLOADER_LUA_H_

