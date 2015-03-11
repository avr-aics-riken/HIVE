#ifndef _CAMERA_LUA_H_
#define _CAMERA_LUA_H_

#include "LuaUtil.h"
#include "Camera.h"
#include "BufferImageData_Lua.h"

class Camera_Lua : public Camera
{
private:
	Camera_Lua(){}
	~Camera_Lua(){}
    
    BufferImageData_Lua* GetImageBuffer() {
        return new BufferImageData_Lua(Camera::GetImageBuffer());
    }
    
    BufferImageData_Lua* GetDepthBuffer() {
        return new BufferImageData_Lua(Camera::GetDepthBuffer());
    }
    
    std::string GetType() {
        return GetTypeString();
    }
    
public:
    LUA_SCRIPTCLASS_BEGIN(Camera_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0 (int, GetScreenWidth)
    LUA_SCRIPTCLASS_METHOD_ARG0 (int, GetScreenHeight)
    LUA_SCRIPTCLASS_METHOD_ARG2 (bool, SetScreenSize, unsigned int, unsigned int)
    LUA_SCRIPTCLASS_METHOD_ARG1 (bool, SetFilename, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG1 (bool, SetNetworkAddress, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG10(bool, LookAt, float, float, float, float, float, float, float, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG4 (bool, ClearColor, float, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG0 (BufferImageData_Lua*,GetImageBuffer)
    LUA_SCRIPTCLASS_METHOD_ARG0 (BufferImageData_Lua*,GetDepthBuffer)
    
    // RenderObject interface
    LUA_SCRIPTCLASS_METHOD_ARG0(std::string, GetType)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetTranslate, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetRotate,float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetScale,float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetVec2,  const char*, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG4(bool, SetVec3,  const char*, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG5(bool, SetVec4,  const char*, float, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool, SetFloat, const char*, float)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(Camera_Lua);

#endif //_CAMERA_LUA_H_
