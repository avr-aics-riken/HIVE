/**
 * @file Volren_Lua.h
 */
#ifndef _VOLREN_LUA_H_
#define _VOLREN_LUA_H_

#include "LuaUtil.h"
#include "Volren.h"
#include "BufferImageData_Lua.h"
#include "BufferVolumeData_Lua.h"
#include <RenderObject/RenderObject.h>
#include <SceneScript/RenderObject_Lua.h>

/**
 * Volren
 */
class Volren_Lua : public Volren
{
public:
  Volren_Lua() {}
  ~Volren_Lua() {}

  bool Create(BufferVolumeData_Lua *vol) {
      Volren::Create(vol);
      return true;
  }

  bool SetClampToEdge(bool s, bool t, bool r) {
      VolumeModel::SetClampToEdge(s, t, r);
      return true;
  }
  
  BufferVolumeData_Lua* GetVolume() {
      return new BufferVolumeData_Lua(VolumeModel::GetVolume());
  }

  // RenderObject interface implementation
  IMPLEMENTATION_RENDEROBJECT_LUA()

	LUA_SCRIPTCLASS_BEGIN(Volren_Lua)
    // from VolumeModel
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetShader, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetClampToEdge,bool,bool,bool)
    LUA_SCRIPTCLASS_METHOD_ARG0(std::string, GetShader)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferVolumeData_Lua, GetVolume)
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, Create, BufferVolumeData_Lua*)
    
    // from Volren
    LUA_SCRIPTCLASS_METHOD_ARG1(int, SetTransferFunctionTexture, BufferImageData_Lua*);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, SetTransferFunctionMin, float);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, SetTransferFunctionMax, float);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, EnableGradientMap, bool);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, SetGradientTexure, BufferImageData_Lua*);
    
    LUA_SCRIPTCLASS_METHOD_ARG1(int, EnableLighting, bool);
    LUA_SCRIPTCLASS_METHOD_ARG4(int, SetLight, float, float, float, float);
    LUA_SCRIPTCLASS_METHOD_ARG3(int, SetAttenuation, float, float, float);
    LUA_SCRIPTCLASS_METHOD_ARG3(int, SetAmbient, float, float, float);
    LUA_SCRIPTCLASS_METHOD_ARG3(int, SetSpecular, float, float, float);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, SetSamples, int);
    LUA_SCRIPTCLASS_METHOD_ARG3(int, SetOffset, float, float, float);
    
    // RenderObject interface
    DECLARE_RENDEROBJECT_LUA_METHOD()
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(Volren_Lua);

#endif // _VOLREN_LUA_H_