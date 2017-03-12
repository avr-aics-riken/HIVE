/**
 * @file PDMLoader_Lua.h
 * PDMLoader Luaラッパー
 */
#ifndef _PDMLOADER_LUA_H_
#define _PDMLOADER_LUA_H_

#include "BufferPointData_Lua.h"
#include "LuaUtil.h"
#include "PdmLoader.h"
/**
 * PDMLoader Luaラッパー
 */
class PDMLoader_Lua : public PDMLoader
{
  public:
	PDMLoader_Lua() {}
	~PDMLoader_Lua() {}

	/// Enable PDMlib's profiling support.
	/// Must be called before `Load`.
	bool EnableProfiling(bool onoff)
	{
		return PDMLoader::EnableProfiling(onoff);
	}

	bool Load(const char *filename, int timeStep = -1,
			  const char *coordinateName = NULL, bool migration = false)
	{
		return PDMLoader::Load(filename, timeStep, coordinateName, migration);
	}

	BufferPointData_Lua *PointData(const char *containerName = "Coordinate",
								   double radius = 1.0)
	{
		RefPtr<BufferPointData> data =
			PDMLoader::PointData(containerName, (float)radius);
		return BufferPointData_Lua::CreateInstance(data);
	}

	BufferExtraData_Lua *ExtraData(const char *containerName)
	{
		RefPtr<BufferExtraData> data = PDMLoader::ExtraData(containerName);
		return BufferExtraData_Lua::CreateInstance(data);
	}

	LUA_SCRIPTCLASS_BEGIN(PDMLoader_Lua)
	LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("PDMLoader", bool, EnableProfiling,
											 bool);
	LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG4("PDMLoader", bool, Load,
											 const char *, int, const char *,
											 bool)
	LUA_SCRIPTCLASS_METHOD_ARG2(BufferPointData_Lua *, PointData, const char *,
								double)
	LUA_SCRIPTCLASS_METHOD_ARG1(BufferExtraData_Lua *, ExtraData, const char *)
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(PDMLoader_Lua);

#endif //_PDMLOADER_LUA_H_
