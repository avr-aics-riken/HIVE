/**
 * @file CDMLoader_Lua.h
 * CDMLoader Luaラッパー
 */
#ifndef _CDMLOADER_LUA_H_
#define _CDMLOADER_LUA_H_

#include "BufferVolumeData_Lua.h"
#include "CdmLoader.h"
#include "LuaUtil.h"
/**
 * CDMLoader Luaラッパー
 */
class CDMLoader_Lua : public CDMLoader
{
  public:
	CDMLoader_Lua() {}
	~CDMLoader_Lua() {}

	int Width() { return Width(); }

	int Height() { return Height(); }

	int Depth() { return Depth(); }

	int Component() { return Component(); }

	double GlobalOffsetX()
	{
		double buf[3];
		CDMLoader::GlobalOffset(buf);
		return buf[0];
	}

	double GlobalOffsetY()
	{
		double buf[3];
		CDMLoader::GlobalOffset(buf);
		return buf[1];
	}

	double GlobalOffsetZ()
	{
		double buf[3];
		CDMLoader::GlobalOffset(buf);
		return buf[2];
	}

	double GlobalRegionX()
	{
		double buf[3];
		CDMLoader::GlobalRegion(buf);
		return buf[0];
	}

	double GlobalRegionY()
	{
		double buf[3];
		CDMLoader::GlobalRegion(buf);
		return buf[1];
	}

	double GlobalRegionZ()
	{
		double buf[3];
		CDMLoader::GlobalRegion(buf);
		return buf[2];
	}

	int GlobalVoxelX()
	{
		int buf[3];
		CDMLoader::GlobalVoxel(buf);
		return buf[0];
	}

	int GlobalVoxelY()
	{
		int buf[3];
		CDMLoader::GlobalVoxel(buf);
		return buf[1];
	}

	int GlobalVoxelZ()
	{
		int buf[3];
		CDMLoader::GlobalVoxel(buf);
		return buf[2];
	}

	int GlobalDivX()
	{
		int buf[3];
		CDMLoader::GlobalDiv(buf);
		return buf[0];
	}

	int GlobalDivY()
	{
		int buf[3];
		CDMLoader::GlobalDiv(buf);
		return buf[1];
	}

	int GlobalDivZ()
	{
		int buf[3];
		CDMLoader::GlobalDiv(buf);
		return buf[2];
	}

	double LocalOffsetX()
	{
		double buf[3];
		CDMLoader::LocalOffset(buf);
		return buf[0];
	}

	double LocalOffsetY()
	{
		double buf[3];
		CDMLoader::LocalOffset(buf);
		return buf[1];
	}

	double LocalOffsetZ()
	{
		double buf[3];
		CDMLoader::LocalOffset(buf);
		return buf[2];
	}

	double LocalRegionX()
	{
		double buf[3];
		CDMLoader::LocalRegion(buf);
		return buf[0];
	}

	double LocalRegionY()
	{
		double buf[3];
		CDMLoader::LocalRegion(buf);
		return buf[1];
	}

	double LocalRegionZ()
	{
		double buf[3];
		CDMLoader::LocalRegion(buf);
		return buf[2];
	}

	int LocalVoxelX()
	{
		int buf[3];
		CDMLoader::LocalVoxel(buf);
		return buf[0];
	}

	int LocalVoxelY()
	{
		int buf[3];
		CDMLoader::LocalVoxel(buf);
		return buf[1];
	}

	int LocalVoxelZ()
	{
		int buf[3];
		CDMLoader::LocalVoxel(buf);
		return buf[2];
	}

	// Head and Tail index starts at 1.
	int HeadIndexX()
	{
		int buf[3];
		CDMLoader::HeadIndex(buf);
		return buf[0];
	}

	int HeadIndexY()
	{
		int buf[3];
		CDMLoader::HeadIndex(buf);
		return buf[1];
	}

	int HeadIndexZ()
	{
		int buf[3];
		CDMLoader::HeadIndex(buf);
		return buf[2];
	}

	int TailIndexX()
	{
		int buf[3];
		CDMLoader::TailIndex(buf);
		return buf[0];
	}

	int TailIndexY()
	{
		int buf[3];
		CDMLoader::TailIndex(buf);
		return buf[1];
	}

	int TailIndexZ()
	{
		int buf[3];
		CDMLoader::TailIndex(buf);
		return buf[2];
	}

	int NumTimeSteps() { return CDMLoader::NumTimeSteps(); }

	int TimeStepAt(int i) { return CDMLoader::GetTimeStep(i); }

	int SetGlobalVoxel(int x, int y, int z)
	{
		int buf[3];
		buf[0] = x;
		buf[1] = y;
		buf[2] = z;
		return CDMLoader::SetGlobalVoxel(buf);
	}

	int SetGlobalDivision(int x, int y, int z)
	{
		int buf[3];
		buf[0] = x;
		buf[1] = y;
		buf[2] = z;
		return CDMLoader::SetGlobalDivision(buf);
	}

	int SetHeadIndex(int x, int y, int z)
	{
		int buf[3];
		buf[0] = x;
		buf[1] = y;
		buf[2] = z;
		return CDMLoader::SetHeadIndex(buf);
	}

	int SetTailIndex(int x, int y, int z)
	{
		int buf[3];
		buf[0] = x;
		buf[1] = y;
		buf[2] = z;
		return CDMLoader::SetTailIndex(buf);
	}

	int SetDivisionMode(int divisionMode, int divisionAxis0 = 0,
						int divisionAxis1 = 1)
	{
		return CDMLoader::SetDivisionMode(divisionMode, divisionAxis0,
										  divisionAxis1);
	}

	bool Load(const char *filename, int timeStepIndex)
	{
		return CDMLoader::Load(filename, timeStepIndex);
	}

	BufferVolumeData_Lua *VolumeData()
	{
		return new BufferVolumeData_Lua(CDMLoader::VolumeData());
	}

	LUA_SCRIPTCLASS_BEGIN(CDMLoader_Lua)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, Width)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, Height)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, Depth)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, Component)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, HeadIndexX)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, HeadIndexY)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, HeadIndexZ)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, TailIndexX)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, TailIndexY)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, TailIndexZ)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, NumTimeSteps)
	LUA_SCRIPTCLASS_METHOD_ARG1(int, TimeStepAt, int)
	// LUA_SCRIPTCLASS_METHOD_ARG3(int, SetHeadIndex, int, int, int)
	// LUA_SCRIPTCLASS_METHOD_ARG3(int, SetTailIndex, int, int, int)
	// LUA_SCRIPTCLASS_METHOD_ARG3(int, SetGlobalVoxel, int, int, int)
	// LUA_SCRIPTCLASS_METHOD_ARG3(int, SetGlobalDivision, int, int, int)
	LUA_SCRIPTCLASS_METHOD_ARG3(int, SetDivisionMode, int, int, int)
	LUA_SCRIPTCLASS_METHOD_ARG0(double, GlobalOffsetX)
	LUA_SCRIPTCLASS_METHOD_ARG0(double, GlobalOffsetY)
	LUA_SCRIPTCLASS_METHOD_ARG0(double, GlobalOffsetZ)
	LUA_SCRIPTCLASS_METHOD_ARG0(double, GlobalRegionX)
	LUA_SCRIPTCLASS_METHOD_ARG0(double, GlobalRegionY)
	LUA_SCRIPTCLASS_METHOD_ARG0(double, GlobalRegionZ)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, GlobalDivX)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, GlobalDivY)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, GlobalDivZ)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, GlobalVoxelX)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, GlobalVoxelY)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, GlobalVoxelZ)
	LUA_SCRIPTCLASS_METHOD_ARG0(double, LocalOffsetX)
	LUA_SCRIPTCLASS_METHOD_ARG0(double, LocalOffsetY)
	LUA_SCRIPTCLASS_METHOD_ARG0(double, LocalOffsetZ)
	LUA_SCRIPTCLASS_METHOD_ARG0(double, LocalRegionX)
	LUA_SCRIPTCLASS_METHOD_ARG0(double, LocalRegionY)
	LUA_SCRIPTCLASS_METHOD_ARG0(double, LocalRegionZ)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, LocalVoxelX)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, LocalVoxelY)
	LUA_SCRIPTCLASS_METHOD_ARG0(int, LocalVoxelZ)
	LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG2("CDMLoader", bool, Load,
											 const char *, int)
	LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua *, VolumeData)
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(CDMLoader_Lua);

#endif //_CDMLOADER_LUA_H_
