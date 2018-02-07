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
#include "BufferLineData_Lua.h"
//#include "BufferTetraData_Lua.h"
#include "BufferVectorData_Lua.h"
/**
 * PrimitiveGenerator Luaラッパー
 */
class PrimitiveGenerator_Lua : public PrimitiveGenerator
{
public:
    PrimitiveGenerator_Lua(){}
    ~PrimitiveGenerator_Lua(){};

    BufferMeshData_Lua* CreateLuaMeshData(BufferMeshData* data) {
        BufferMeshData_Lua * newdata = BufferMeshData_Lua::CreateInstance(data);
        //delete data;;
        return newdata;
    }

    BufferPointData_Lua* CreateLuaPointData(BufferPointData* data) {
        BufferPointData_Lua * newdata = BufferPointData_Lua::CreateInstance(data);
        //delete data;
        return newdata;
    }

    BufferLineData_Lua* CreateLuaLineData(BufferLineData* data) {
        BufferLineData_Lua * newdata = BufferLineData_Lua::CreateInstance(data);
        //delete data;
        return newdata;
    }

    BufferVectorData_Lua* CreateLuaVectorData(BufferVectorData* data) {
        BufferVectorData_Lua * newdata = BufferVectorData_Lua::CreateInstance(data);
        //delete data;
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
        const std::vector<LuaTable>&          lt       = tbl.GetTable();
        std::vector<LuaTable>::const_iterator ite      = lt.begin();
        const std::vector<LuaTable>::const_iterator ite_end  = lt.end();
        std::vector<float> buf;
        while(ite != ite_end) {
            double param = ite->GetNumber();
            buf.push_back(param);
            ++ite;
        }
        return CreateLuaPointData(PrimitiveGenerator::SphereList(&buf[0], num, radius));
    }


    BufferLineData_Lua* LineList(LuaTable tbl, float num, float radius) {
        const std::vector<LuaTable>&          lt       = tbl.GetTable();
        std::vector<LuaTable>::const_iterator ite      = lt.begin();
        const std::vector<LuaTable>::const_iterator ite_end  = lt.end();
        std::vector<float> buf;
        while(ite != ite_end) {
            double param = ite->GetNumber();
            buf.push_back(param);
            ++ite;
        }
        return CreateLuaLineData(PrimitiveGenerator::LineList(&buf[0], num, radius));
    }

    BufferVectorData_Lua* VectorArrowList(LuaTable p_tbl, LuaTable d_tbl, float num) {
        const std::vector<LuaTable>&          p_lt       = p_tbl.GetTable();
        std::vector<LuaTable>::const_iterator p_ite      = p_lt.begin();
        const std::vector<LuaTable>::const_iterator p_ite_end  = p_lt.end();

        const std::vector<LuaTable>&          d_lt       = d_tbl.GetTable();
        std::vector<LuaTable>::const_iterator d_ite      = d_lt.begin();
        const std::vector<LuaTable>::const_iterator d_ite_end  = d_lt.end();

        std::vector<float> p_buf;
        std::vector<float> d_buf;

        // Assume size of p_tbl and d_tbl are same.
        while(p_ite != p_ite_end) {
            double p_param = p_ite->GetNumber();
            double d_param = d_ite->GetNumber();
            p_buf.push_back(p_param);
            d_buf.push_back(d_param);
            ++p_ite;
            ++d_ite;
        }
        return CreateLuaVectorData(PrimitiveGenerator::VectorArrowList(&p_buf[0], &d_buf[0], num));
    }


    BufferMeshData_Lua* TriangleList(LuaTable tbl, float num) {
        const std::vector<LuaTable>&          lt       = tbl.GetTable();
        std::vector<LuaTable>::const_iterator ite      = lt.begin();
        const std::vector<LuaTable>::const_iterator ite_end  = lt.end();
        std::vector<float> buf;
        while(ite != ite_end) {
            double param = ite->GetNumber();
            buf.push_back(param);
            ++ite;
        }
        return CreateLuaMeshData(PrimitiveGenerator::TriangleList(&buf[0], num));
    }

    LUA_SCRIPTCLASS_BEGIN(PrimitiveGenerator_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG2(BufferMeshData_Lua*,   Quad,float,float)
    LUA_SCRIPTCLASS_METHOD_ARG1(BufferPointData_Lua*,  Sphere,float)
    LUA_SCRIPTCLASS_METHOD_ARG3(BufferMeshData_Lua*,   Cube,float,float,float)
    LUA_SCRIPTCLASS_METHOD_ARG1(BufferMeshData_Lua*,   Teapot,float)
    LUA_SCRIPTCLASS_METHOD_ARG3(BufferPointData_Lua*,  PointList,LuaTable, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(BufferLineData_Lua*,   LineList,LuaTable, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG2(BufferMeshData_Lua*,   TriangleList,LuaTable, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(BufferVectorData_Lua*,   VectorArrowList,LuaTable, LuaTable, float)
    //LUA_SCRIPTCLASS_METHOD_ARG0(BufferLineData_Lua*,   LineData)
    //LUA_SCRIPTCLASS_METHOD_ARG0(BufferTetraData_Lua*,  TetraData)
    //LUA_SCRIPTCLASS_METHOD_ARG0(BufferVectorData_Lua*, NormalData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(PrimitiveGenerator_Lua);

#endif //_PRIMITIVEGENERATOR_LUA_H_

