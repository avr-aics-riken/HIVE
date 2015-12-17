/**
 * @file ImageLoader_Lua.h
 * ImageLoader Luaラッパー
 */
#ifndef _IMAGELOADER_LUA_H_
#define _IMAGELOADER_LUA_H_

#include "LuaUtil.h"
#include "../Image/ImageLoader.h"
#include "BufferImageData_Lua.h"
/**
 * ImageLoader Luaラッパー
 */
class ImageLoader_Lua : public ImageLoader
{
public:
    ImageLoader_Lua() {}
    ~ImageLoader_Lua() {}

    bool Load(const char* filename) {
        return ImageLoader::Load(filename);
    }

    BufferImageData_Lua* ImageData() {
        return new BufferImageData_Lua(ImageLoader::ImageData());
    }

    LUA_SCRIPTCLASS_BEGIN(ImageLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool,Load,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferImageData_Lua*,ImageData)
    LUA_SCRIPTCLASS_METHOD_ARG0(ImageLoader::Buffer,ImageBuffer)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,ImageBufferSize)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(ImageLoader_Lua);

#endif //_IMAGELOADER_LUA_H_

