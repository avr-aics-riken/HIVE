/**
 * @file BufferImageData_Lua.h
 * BufferImageData Luaラッパー
 */
#ifndef _BUFFERIMAGEDATA_LUA_H_
#define _BUFFERIMAGEDATA_LUA_H_

#include "BufferImageData.h"
#include "Buffer.h"

/**
 * BufferImageData Luaラッパー
 */
class BufferImageData_Lua : public BufferImageData
{
public:
    BufferImageData_Lua(BufferImageData* data) : BufferImageData(data) { }
    BufferImageData_Lua(const BufferImageData* data) : BufferImageData(data) { }
    BufferImageData_Lua() {}
    
    int GetWidth() const { return Width(); };
    int GetHeight() const { return Height(); };
    //const char* GetFormat() const { return Format() };
    int GetSize() const { return Bytes(); };
    const char* GetBuffer() const { return reinterpret_cast<const char*>(ImageBuffer()->GetBuffer()); };

    ~BufferImageData_Lua() {}
    
public:
    static BufferImageData_Lua* CreateInstance(BufferImageData* bufferImageData = 0);
    LUA_SCRIPTCLASS_BEGIN(BufferImageData_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0(int, GetWidth)
    LUA_SCRIPTCLASS_METHOD_ARG0(int, GetHeight)
    //LUA_SCRIPTCLASS_METHOD_ARG0(const char*, GetFormat)
    LUA_SCRIPTCLASS_METHOD_ARG0(int, GetSize)
    LUA_SCRIPTCLASS_METHOD_ARG0(const char*, GetBuffer)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(BufferImageData_Lua);


#ifdef CPP_IMPL_INSTANCE

BufferImageData_Lua* BufferImageData_Lua::CreateInstance(BufferImageData* bufferImageData)
{
    if (bufferImageData) {
        return new BufferImageData_Lua(bufferImageData);
    } else {
        return new BufferImageData_Lua();
    }
}

#endif

#endif //_BUFFERIMAGEDATA_LUA_H_

