/**
 * @file UDMLoader_Lua.h
 * UDMLoader Luaラッパー
 */
#ifndef _UDMLOADER_LUA_H_
#define _UDMLOADER_LUA_H_

#include "LuaUtil.h"
#include "UdmLoader.h"
#include "BufferVolumeData_Lua.h"
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

    LUA_SCRIPTCLASS_BEGIN(UDMLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,NumTimeSteps)
    LUA_SCRIPTCLASS_METHOD_ARG1(int,TimeStepAt,int)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool,Load,const char*,int)
	// @todo
    //LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(UDMLoader_Lua);

#endif //_UDMLOADER_LUA_H_
