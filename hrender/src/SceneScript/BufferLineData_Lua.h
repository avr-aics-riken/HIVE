#ifndef _BUFFERLINEDATA_LUA_H_
#define _BUFFERLINEDATA_LUA_H_

#include "BufferLineData.h"

class BufferLineData_Lua : public BufferLineData
{
public:
    BufferLineData_Lua(BufferLineData* pnt) : BufferLineData(pnt) { }
    BufferLineData_Lua() {}
    ~BufferLineData_Lua() {}
    LUA_SCRIPTCLASS_BEGIN(BufferLineData_Lua)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(BufferLineData_Lua);

#endif //_BUFFERLINEDATA_LUA_H_

