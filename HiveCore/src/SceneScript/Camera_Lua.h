/**
 * @file Camera_Lua.h
 * Camera Luaラッパー
 */
#ifndef _CAMERA_LUA_H_
#define _CAMERA_LUA_H_

#include "../RenderObject/Camera.h"
#include "LuaUtil.h"
#include "BufferImageData_Lua.h"
#include "RenderObject_Lua.h"
/**
 * Camera Luaラッパー
 */
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
    
    LuaTable GetPosition() {
        const VX::Math::vec3 pos = Camera::GetPosition();
        return LuaTable(pos.x, pos.y, pos.z);
    }
    LuaTable GetTarget() const {
        const VX::Math::vec3 tar = Camera::GetTarget();
        return LuaTable(tar.x, tar.y, tar.z);
    }
    LuaTable GetUp() const {
        const VX::Math::vec3 up = Camera::GetUp();
        return LuaTable(up.x, up.y, up.z);
    }

    LuaTable GetClearColor() {
        const float* clr = Camera::GetClearColor();
        return LuaTable(clr[0], clr[1], clr[2], clr[3]);
    }

    LuaTable GetScreenSize() {
        return LuaTable(Camera::GetScreenWidth(), Camera::GetScreenHeight());
    }
    
    LuaTable ConvertToScreenSpace(float x, float y, float z) {
        VX::Math::vec3 pos = Camera::ConvertToScreenSpace(x, y, z);
        return LuaTable(pos.x, pos.y, pos.z);
    }
        
    // RenderObject methods
    IMPLEMENTATION_RENDEROBJECT_LUA()
    
public:
    LUA_SCRIPTCLASS_BEGIN(Camera_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG0 (int, GetScreenWidth)
    LUA_SCRIPTCLASS_METHOD_ARG0 (int, GetScreenHeight)
    LUA_SCRIPTCLASS_METHOD_ARG2 (bool, SetScreenSize, unsigned int, unsigned int)
    LUA_SCRIPTCLASS_METHOD_ARG1 (bool, SetFilename, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG1 (bool, SetImageBufferFormat, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG1 (bool, SetDepthFilename, const std::string&)
    LUA_SCRIPTCLASS_METHOD_ARG10(bool, LookAt, float, float, float, float, float, float, float, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG4 (bool, ClearColor, float, float, float, float)
    LUA_SCRIPTCLASS_METHOD_ARG0 (BufferImageData_Lua*,GetImageBuffer)
    LUA_SCRIPTCLASS_METHOD_ARG0 (BufferImageData_Lua*,GetDepthBuffer)
    LUA_SCRIPTCLASS_METHOD_ARG0 (std::string, GetOutputFile)
    LUA_SCRIPTCLASS_METHOD_ARG0 (std::string, GetDepthOutputFile)
    LUA_SCRIPTCLASS_METHOD_ARG0 (std::string , GetImageBufferFormat)
    
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetPosition)
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetTarget)
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetUp)
    LUA_SCRIPTCLASS_METHOD_ARG0(float,    GetFov)
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetClearColor)
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetScreenSize)
    
    LUA_SCRIPTCLASS_METHOD_ARG3(LuaTable, ConvertToScreenSpace, float, float, float);

    
    // RenderObject interface
    DECLARE_RENDEROBJECT_LUA_METHOD()
    LUA_SCRIPTCLASS_END();
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(Camera_Lua);

#endif //_CAMERA_LUA_H_
