/**
 * @file PrimitiveGenerator_Lua.h
 * PrimitiveGenerator Luaラッパー
 */
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
/**
 * PrimitiveGenerator Luaラッパー
 */
class PrimitiveGenerator_Lua : public PrimitiveGenerator
{
public:
    PrimitiveGenerator_Lua(){}
    ~PrimitiveGenerator_Lua(){};

    BufferMeshData_Lua* CreateLuaMeshData(BufferMeshData* data) {
        BufferMeshData_Lua * newdata = new BufferMeshData_Lua(data);
        delete data;
        return newdata;
    }

    BufferPointData_Lua* CreateLuaPointData(BufferPointData* data) {
        BufferPointData_Lua * newdata = new BufferPointData_Lua(data);
        delete data;
        return newdata;
    }

    BufferMeshData_Lua* Quad(float width, float height) {
        return CreateLuaMeshData(PrimitiveGenerator::Quad(width, height));
    }

    BufferPointData_Lua* Sphere(float radius) {
        return CreateLuaPointData(PrimitiveGenerator::Sphere(radius));
    }

    BufferMeshData_Lua* Cube(float width, float height, float depth) {
        return CreateLuaMeshData(PrimitiveGenerator::Cube(width, height, depth));
    }
    BufferMeshData_Lua* Teapot(float scale) {
        return CreateLuaMeshData(PrimitiveGenerator::Teapot(scale));
    }

    BufferPointData_Lua* PointList(LuaTable tbl, float num, float radius) {
        std::vector<LuaTable>           lt       = tbl.GetTable();
        std::vector<LuaTable>::iterator ite      = lt.begin();
        std::vector<LuaTable>::iterator ite_end  = lt.end();
        std::vector<float> buf;
        while(ite != ite_end) {
            double param = ite->GetNumber();
            buf.push_back(param);
            ite++;
        }
        return new BufferPointData_Lua(PrimitiveGenerator::SphereList(&buf[0], num, radius));
    }

    LUA_SCRIPTCLASS_BEGIN(PrimitiveGenerator_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG2(BufferMeshData_Lua*,   Quad,float,float)
    LUA_SCRIPTCLASS_METHOD_ARG1(BufferPointData_Lua*,  Sphere,float)
    LUA_SCRIPTCLASS_METHOD_ARG3(BufferMeshData_Lua*,   Cube,float,float,float)
    LUA_SCRIPTCLASS_METHOD_ARG1(BufferMeshData_Lua*,   Teapot,float)
    LUA_SCRIPTCLASS_METHOD_ARG3(BufferPointData_Lua*,  PointList,LuaTable, float, float)
    //LUA_SCRIPTCLASS_METHOD_ARG0(BufferLineData_Lua*,   LineData)
    //LUA_SCRIPTCLASS_METHOD_ARG0(BufferTetraData_Lua*,  TetraData)
    //LUA_SCRIPTCLASS_METHOD_ARG0(BufferVectorData_Lua*, NormalData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(PrimitiveGenerator_Lua);

#endif //_PRIMITIVEGENERATOR_LUA_H_

