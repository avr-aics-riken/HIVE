/**
 * @file Volren_Lua.h
 */
#ifndef _VOLREN_LUA_H_
#define _VOLREN_LUA_H_

#include "LuaUtil.h"
#include "Volren.h"
#include "BufferImageData_Lua.h"

/**
 * Volren
 */
class Volren_Lua : public Volren
{
  public:
	Volren_Lua() {}
	~Volren_Lua() {}

	LUA_SCRIPTCLASS_BEGIN(Volren_Lua)
	//LUA_SCRIPTCLASS_METHOD_ARG1(int, Parse, const char*)
    LUA_SCRIPTCLASS_METHOD_ARG1(int, SetTransferFunctionTexture, BufferImageData_Lua*);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, SetTransferFunctionMin, float);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, SetTransferFunctionMax, float);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, SetGradientTexure, BufferImageData_Lua*);
    
    LUA_SCRIPTCLASS_METHOD_ARG1(int, EnableLighting, bool);
    LUA_SCRIPTCLASS_METHOD_ARG4(int, SetLight, float, float, float, float);
    LUA_SCRIPTCLASS_METHOD_ARG3(int, SetAttenuation, float, float, float);
    LUA_SCRIPTCLASS_METHOD_ARG3(int, SetAmbient, float, float, float);
    LUA_SCRIPTCLASS_METHOD_ARG3(int, SetSpecular, float, float, float);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, SetSamples, int);
    LUA_SCRIPTCLASS_METHOD_ARG3(int, SetOffset, float, float, float);
    
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(Volren_Lua);

#endif // _VOLREN_LUA_H_