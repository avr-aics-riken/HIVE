/**
 * @file ImageFilter_Lua.h
 * ImageFilter Luaラッパー
 */
#ifndef _IMAGEFILTER_LUA_H_
#define _IMAGEFILTER_LUA_H_

#include "LuaUtil.h"
#include "ImageFilter.h"
#include "BufferImageData_Lua.h"
/**
 * ImageFilter Luaラッパー
 */
class ImageFilter_Lua : public ImageFilter
{
public:
    ImageFilter_Lua() {}
    ~ImageFilter_Lua() {}

    BufferImageData_Lua* Filter(unsigned int operation, BufferImageData_Lua* left, BufferImageData_Lua* right, float opacity) {
        return new BufferImageData_Lua(ImageFilter::Filter(operation, left, right, opacity));
    }

    LUA_SCRIPTCLASS_BEGIN(ImageFilter_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG4(BufferImageData_Lua*, Filter,unsigned int, BufferImageData_Lua*, BufferImageData_Lua*, float);
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(ImageFilter_Lua);

#endif //_IMAGEFILTER_LUA_H_

