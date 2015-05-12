/**
 * @file MetaBinary_Lua.h
 * MetaBinary Luaラッパー
 */
#ifndef _METABINARY_LUA_H_
#define _METABINARY_LUA_H_

#include "LuaUtil.h"
#include "MetaBinary.h"
/**
 * MetaBinary Luaラッパー
 */
class MetaBinary_Lua : public MetaBinary
{
private:
	MetaBinary_Lua(){}
	~MetaBinary_Lua(){}

public:
    LUA_SCRIPTCLASS_BEGIN(MetaBinary_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG3 (bool, Create, const std::string&, const MetaBinary::Buffer, int)
    LUA_SCRIPTCLASS_METHOD_ARG0 (const MetaBinary::Buffer, BinaryBuffer)
    LUA_SCRIPTCLASS_METHOD_ARG0 (int, BinaryBufferSize)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(MetaBinary_Lua);

#endif //_METABINARY_LUA_H_
