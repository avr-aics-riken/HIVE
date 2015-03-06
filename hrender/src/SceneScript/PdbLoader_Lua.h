#ifndef _PDBLOADER_LUA_H_
#define _PDBLOADER_LUA_H_

#include "LuaUtil.h"
#include "Buffer.h"
#include "BufferPointData_Lua.h"
#include "PdbLoader.h"

//------//------//------//------//------//------//------//------//------
//
//------//------//------//------//------//------//------//------//------
class PDBLoader_Lua : public PDBLoader
{
public:
    PDBLoader_Lua(){}
    ~PDBLoader_Lua(){};

    bool Load(const char* filename){
        return PDBLoader::Load(filename);
    }

    BufferPointData_Lua* BallData() {
        return new BufferPointData_Lua(PDBLoader::BallData());
    }

    BufferLineData_Lua* StickData() {
        return new BufferLineData_Lua(PDBLoader::StickData());
    }

    LUA_SCRIPTCLASS_BEGIN(PDBLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool,Load,const char*)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferPointData_Lua*, BallData)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferLineData_Lua*, StickData)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(PDBLoader_Lua);

#endif //_PDBLOADER_LUA_H_

