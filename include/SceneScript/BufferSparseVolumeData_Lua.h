/**
 * @file BufferSparseVolumeData_Lua.h
 * BufferSparseVolumeData Luaラッパー
 */
#ifndef _BUFFERSPARSEVOLUMEDATA_LUA_H_
#define _BUFFERSPARSEVOLUMEDATA_LUA_H_

#include "BufferSparseVolumeData.h"
/**
 * BufferSparseVolumeData Luaラッパー
 */
class BufferSparseVolumeData_Lua : public BufferSparseVolumeData
{
protected:
    BufferSparseVolumeData_Lua(BufferSparseVolumeData* vol) : BufferSparseVolumeData(vol) { }
    BufferSparseVolumeData_Lua() {}
    ~BufferSparseVolumeData_Lua() {}
    
public:
    static BufferSparseVolumeData_Lua* CreateInstance(BufferSparseVolumeData* bufferSparseVolumeData = 0);
    LUA_SCRIPTCLASS_BEGIN(BufferSparseVolumeData_Lua)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(BufferSparseVolumeData_Lua);

#endif //_BUFFERSPARSEVOLUMEDATA_LUA_H_

