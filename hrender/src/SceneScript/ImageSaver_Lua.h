/**
 * @file ImageSaver_Lua.h
 * ImageSaver Luaラッパー
 */
#ifndef _IMAGESAVER_LUA_H_
#define _IMAGESAVER_LUA_H_

#include <string>
#include "LuaUtil.h"
#include "../Image/ImageSaver.h"
#include "BufferImageData_Lua.h"

class ImageSaver_Lua : public ImageSaver
{
public:
    ImageSaver_Lua() {}
    ~ImageSaver_Lua() {}

    bool Save(const char* filename, BufferImageData_Lua* data) {
        return ImageSaver::Save(filename, data);
    }

    LUA_SCRIPTCLASS_BEGIN(ImageSaver_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool,Save,const char*,BufferImageData_Lua*)
    LUA_SCRIPTCLASS_METHOD_ARG2(const ImageSaver::Buffer,SaveMemory,unsigned int,BufferImageData_Lua*)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,MemorySize)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(ImageSaver_Lua);

#endif //_IMAGESAVER_LUA_H_

