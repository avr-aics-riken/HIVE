/**
 * @file BufferExtraData_Lua.h
 * BufferExtraData Luaラッパー
 */
#ifndef _BUFFEREXTRADATA_LUA_H_
#define _BUFFEREXTRADATA_LUA_H_

#include "BufferExtraData.h"

/**
 * BufferExtraData Luaラッパー
 */
class BufferExtraData_Lua : public BufferExtraData
{
public:
    BufferExtraData_Lua(BufferExtraData* edata) : BufferExtraData(edata) { }
    BufferExtraData_Lua() {}
    ~BufferExtraData_Lua() {}
    
public:
    static BufferExtraData_Lua* CreateInstance(BufferExtraData* bufferExtraData = 0);
    LUA_SCRIPTCLASS_BEGIN(BufferExtraData_Lua)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(BufferExtraData_Lua);


#ifdef CPP_IMPL_INSTANCE

BufferExtraData_Lua* BufferExtraData_Lua::CreateInstance(BufferExtraData* bufferExtraData)
{
    if (bufferExtraData) {
        return new BufferExtraData_Lua(bufferExtraData);
    } else {
        return new BufferExtraData_Lua();
    }
}

#endif

#endif //_BUFFEREXTRADATA_LUA_H_

