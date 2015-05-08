/**
 * @file PDMLoader_Lua.h
 * PDMLoader Luaラッパー
 */
#ifndef _PDMLOADER_LUA_H_
#define _PDMLOADER_LUA_H_

#include "LuaUtil.h"
#include "PdmLoader.h"
#include "BufferPointData_Lua.h"
/**
 * PDMLoader Luaラッパー
 */
class PDMLoader_Lua : public PDMLoader
{
public:
    PDMLoader_Lua() {}
    ~PDMLoader_Lua() {}

    bool Load(const char* filename, int timeStep) {
        return PDMLoader::Load(filename, timeStep);
    }

    BufferPointData_Lua* PointData() {
        return new BufferPointData_Lua(PDMLoader::PointData());
    }

    LUA_SCRIPTCLASS_BEGIN(PDMLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool,Load,const char*,int)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferPointData_Lua*,PointData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(PDMLoader_Lua);

#endif //_PDMLOADER_LUA_H_

