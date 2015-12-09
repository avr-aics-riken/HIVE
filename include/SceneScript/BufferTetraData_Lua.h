/**
 * @file BufferTetraData_Lua.h
 * BufferTetraData Luaラッパー
 */
#ifndef _BUFFERTETRADATA_LUA_H_
#define _BUFFERTETRADATA_LUA_H_

#include "BufferTetraData.h"
/**
 * BufferTetraData Luaラッパー
 */
class BufferTetraData_Lua : public BufferTetraData
{
protected:
    BufferTetraData_Lua(BufferTetraData* vol) : BufferTetraData(vol) { }
    BufferTetraData_Lua() {}
    ~BufferTetraData_Lua() {}

public:
    static BufferTetraData_Lua* CreateInstance(BufferTetraData* bufferTetraData = 0);
    LUA_SCRIPTCLASS_BEGIN(BufferTetraData_Lua)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(BufferTetraData_Lua);


#ifdef CPP_IMPL_INSTANCE

BufferTetraData_Lua* BufferTetraData_Lua::CreateInstance(BufferTetraData* bufferTetraData)
{
    if (bufferTetraData) {
        return new BufferTetraData_Lua(bufferTetraData);
    } else {
        return new BufferTetraData_Lua();
    }
}

#endif

#endif //_BUFFERTETRADATA_LUA_H_

