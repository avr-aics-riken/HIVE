/**
 * @file UDMLoader_Lua.h
 * UDMLoader Luaラッパー
 */
#ifndef _UDMLOADER_LUA_H_
#define _UDMLOADER_LUA_H_

#include "LuaUtil.h"
#include "UdmLoader.h"
#include "BufferMeshData_Lua.h"
#include "BufferTetraData_Lua.h"
#include "BufferSolidData_Lua.h"
/**
 * UDMLoader Luaラッパー
 */
class UDMLoader_Lua : public UDMLoader
{
public:
    UDMLoader_Lua() {}
    ~UDMLoader_Lua() {}

    int NumTimeSteps() {
      return UDMLoader::NumTimeSteps();
    }

    int TimeStepAt(int i) {
      return UDMLoader::GetTimeStep(i);
    }

    bool Load(const char* filename, int stepno) {
        return UDMLoader::Load(filename, stepno);
    }

    BufferMeshData_Lua* MeshData() {
        RefPtr<BufferMeshData> mesh = UDMLoader::MeshData();
        return BufferMeshData_Lua::CreateInstance(mesh);
    }

    BufferTetraData_Lua* TetraData() {
        RefPtr<BufferTetraData> data = UDMLoader::TetraData();
        return BufferTetraData_Lua::CreateInstance(data);
    }

    BufferSolidData_Lua* SolidData(int solidType) {
        return new BufferSolidData_Lua(UDMLoader::SolidData(solidType));
    }

    BufferExtraData_Lua* ExtraData(const char *name) {
        RefPtr<BufferExtraData> data = UDMLoader::ExtraData(name);
        return BufferExtraData_Lua::CreateInstance(data);
    }

    LUA_SCRIPTCLASS_BEGIN(UDMLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,NumTimeSteps)
    LUA_SCRIPTCLASS_METHOD_ARG1(int,TimeStepAt,int)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG2("UDMLoader",bool,Load,const char*,int)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferMeshData_Lua*, MeshData)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferTetraData_Lua*, TetraData)
    LUA_SCRIPTCLASS_METHOD_ARG1(BufferSolidData_Lua*, SolidData, int)
    LUA_SCRIPTCLASS_METHOD_ARG1(BufferExtraData_Lua*, ExtraData, const char*)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(UDMLoader_Lua);

#endif //_UDMLOADER_LUA_H_
