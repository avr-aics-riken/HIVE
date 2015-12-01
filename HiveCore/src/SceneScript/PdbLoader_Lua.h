/**
 * @file PDBLoader_Lua.h
 * PDBLoader Luaラッパー
 */
#ifndef _PDBLOADER_LUA_H_
#define _PDBLOADER_LUA_H_

#include "LuaUtil.h"
#include "Buffer.h"
#include "BufferPointData_Lua.h"
#include "PdbLoader.h"
/**
 * PDBLoader Luaラッパー
 */
class PDBLoader_Lua : public PDBLoader
{
public:
    PDBLoader_Lua(){}
    ~PDBLoader_Lua(){};

    bool Load(const char* filename, bool generateBond = false){
        return PDBLoader::Load(filename, generateBond);
    }

    BufferPointData_Lua* BallData() {
        return new BufferPointData_Lua(PDBLoader::BallData());
    }

    BufferLineData_Lua* StickData() {
        return new BufferLineData_Lua(PDBLoader::StickData());
    }

	int NumAtoms() {
        return PDBLoader::NumAtoms();
    }

	bool SetMaterial(int i, float matID) {
        PDBLoader::SetMaterial(i-1, matID);
		return true;
	}

	std::string AtomElementSymbol(int i) {
        return PDBLoader::AtomElementSymbol(i-1);
    }

    LUA_SCRIPTCLASS_BEGIN(PDBLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool,Load,const char*,bool)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferPointData_Lua*, BallData)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferLineData_Lua*, StickData)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,NumAtoms)
    LUA_SCRIPTCLASS_METHOD_ARG1(std::string,AtomElementSymbol,int)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool,SetMaterial,int,float)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(PDBLoader_Lua);

#endif //_PDBLOADER_LUA_H_

