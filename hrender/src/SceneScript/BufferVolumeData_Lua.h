#ifndef _BUFFERVOLUMEDATA_LUA_H_
#define _BUFFERVOLUMEDATA_LUA_H_

#include "BufferVolumeData.h"

class BufferVolumeData_Lua : public BufferVolumeData
{
public:
    BufferVolumeData_Lua(BufferVolumeData* vol) : BufferVolumeData(vol) { }
    BufferVolumeData_Lua() {}
    ~BufferVolumeData_Lua() {}
    LUA_SCRIPTCLASS_BEGIN(BufferVolumeData_Lua)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(BufferVolumeData_Lua);

#endif //_BUFFERVOLUMEDATA_LUA_H_

