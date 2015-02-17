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
    
public:
    LUA_SCRIPTCLASS_BEGIN(Camera_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG2 (bool, SetScreenSize, unsigned int, unsigned int)
    LUA_SCRIPTCLASS_METHOD_ARG1 (bool, SetFilename, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG1 (bool, SetNetworkAddress, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG10(bool, LookAt, float, float, float, float, float, float, float, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG4 (bool, ClearColor, float, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG0 (BufferImageData_Lua*,GetImageBuffer)
    LUA_SCRIPTCLASS_METHOD_ARG0 (BufferImageData_Lua*,GetDepthBuffer)
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(Camera_Lua);

#endif //_CAMERA_LUA_H_
