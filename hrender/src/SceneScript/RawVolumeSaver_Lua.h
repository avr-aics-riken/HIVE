/**
 * @file RawVolumeSaver_Lua.h
 * RawVolumeSaver Luaラッパー
 */
#ifndef _RAWVOLUMESAVER_LUA_H_
#define _RAWVOLUMESAVER_LUA_H_

#include "LuaUtil.h"
#include "RawVolumeSaver.h"
#include "BufferVolumeData_Lua.h"
/**
 * RawVolumeSaver Luaラッパー
 */
class RawVolumeSaver_Lua : public RawVolumeSaver
{
public:
    RawVolumeSaver_Lua() {}
    ~RawVolumeSaver_Lua() {}

    bool Save(const char* filename, bool appendFooter) {
        return RawVolumeSaver::Save(filename, appendFooter);
    }

    bool SetVolumeData(BufferVolumeData_Lua* volume) {
        RawVolumeSaver::SetVolumeData(volume);
        return true;
    }

    LUA_SCRIPTCLASS_BEGIN(RawVolumeSaver_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool,Save,const char*,bool)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool,SetVolumeData,BufferVolumeData_Lua*)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(RawVolumeSaver_Lua);

#endif //_RAWVOLUMESAVER_LUA_H_

