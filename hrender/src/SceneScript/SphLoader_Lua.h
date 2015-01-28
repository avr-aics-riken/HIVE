#ifndef _SPHLOADER_LUA_H_
#define _SPHLOADER_LUA_H_

#include "LuaUtil.h"
#include "SphLoader.h"
#include "BufferVolumeData_Lua.h"

class SPHLoader_Lua : public SPHLoader
{
public:
	SPHLoader_Lua();
	~SPHLoader_Lua();
	void Clear();
	bool Load(const char* filename);
	int Width();
	int Height();
	int Depth();
	int Component();
	BufferVolumeData_Lua* VolumeData();
	LUA_SCRIPTCLASS_BEGIN(SPHLoader_Lua)
	LUA_SCRIPTCLASS_METHOD_ARG0(int,Width)
	LUA_SCRIPTCLASS_METHOD_ARG0(int,Height)
	LUA_SCRIPTCLASS_METHOD_ARG0(int,Depth)
	LUA_SCRIPTCLASS_METHOD_ARG0(int,Component)
	LUA_SCRIPTCLASS_METHOD_ARG1(bool,Load,const char*)
	LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua*,VolumeData)
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(SPHLoader_Lua);

#endif //_SPHLOADER_LUA_H_

