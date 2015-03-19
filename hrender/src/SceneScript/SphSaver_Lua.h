/**
 * @file SPHSaver_Lua.h
 * SPHSaver Luaラッパー
 */
#ifndef _SPHSAVER_LUA_H_
#define _SPHSAVER_LUA_H_

#include "LuaUtil.h"
#include "SphSaver.h"
#include "BufferVolumeData_Lua.h"
/**
 * SPHSaver Luaラッパー
 */
class SPHSaver_Lua : public SPHSaver
{
public:
    SPHSaver_Lua() {}
    ~SPHSaver_Lua() {}

    bool Save(const char* filename) {
        return SPHSaver::Save(filename);
    }

    bool SetVolumeData(BufferVolumeData_Lua* volume) {
        SPHSaver::SetVolumeData(volume);
        return true;
    }

    LUA_SCRIPTCLASS_BEGIN(SPHSaver_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool,Save,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool,SetVolumeData,BufferVolumeData_Lua*)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(SPHSaver_Lua);

#endif //_SPHLOADER_LUA_H_

