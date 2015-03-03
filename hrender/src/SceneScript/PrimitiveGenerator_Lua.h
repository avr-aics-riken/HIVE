#ifndef _PRIMITIVEGENERATOR_LUA_H_
#define _PRIMITIVEGENERATOR_LUA_H_

#include "LuaUtil.h"
#include "Buffer.h"
#include "PrimitiveGenerator.h"
#include "BufferMeshData_Lua.h"
#include "BufferPointData_Lua.h"
//#include "BufferLineData_Lua.h"
//#include "BufferTetraData_Lua.h"
//#include "BufferVectorData_Lua.h"

//------//------//------//------//------//------//------//------//------
//
//------//------//------//------//------//------//------//------//------
class PrimitiveGenerator_Lua : public PrimitiveGenerator
{
public:
    PrimitiveGenerator_Lua(){}
    ~PrimitiveGenerator_Lua(){};

    BufferMeshData_Lua* Quad(float width, float height) {
        return new BufferMeshData_Lua(PrimitiveGenerator::Quad(width, height));
    }

    BufferPointData_Lua* Sphere(float radius) {
        return new BufferPointData_Lua(PrimitiveGenerator::Sphere(radius));
    }

    LUA_SCRIPTCLASS_BEGIN(PrimitiveGenerator_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG2(BufferMeshData_Lua*,   Quad,float,float)
    LUA_SCRIPTCLASS_METHOD_ARG1(BufferPointData_Lua*,  Sphere,float)
    //LUA_SCRIPTCLASS_METHOD_ARG0(BufferLineData_Lua*,   LineData)
    //LUA_SCRIPTCLASS_METHOD_ARG0(BufferTetraData_Lua*,  TetraData)
    //LUA_SCRIPTCLASS_METHOD_ARG0(BufferVectorData_Lua*, NormalData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(PrimitiveGenerator_Lua);

#endif //_PRIMITIVEGENERATOR_LUA_H_

