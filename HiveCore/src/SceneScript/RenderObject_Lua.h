/**
 * @file RenderObject_Lua.h
 * RenderObject Luaラッパー
 */
#ifndef _RENDEROBJECT_LUA_H_
#define _RENDEROBJECT_LUA_H_
#include "Buffer.h"
#include "BufferImageData_Lua.h"
#include "BufferExtraData_Lua.h"


#define IMPLEMENTATION_RENDEROBJECT_LUA() \
    bool SetTransformMatrix(LuaTable t) { \
        if (t.GetType() != LuaTable::TYPE_ARRAY) \
            return false; \
        const std::vector<LuaTable>& tbl = t.GetTable(); \
        if (tbl.size() != 16) \
            return false; \
        float m[16]; \
        for (int i = 0; i < 16; ++i) { \
            m[i] = tbl[i].GetNumber(); \
        } \
        return RenderObject::SetTransformMatrix(&m[0]); \
    } \
    std::string GetType() { \
        return GetTypeString(); \
    } \
    LuaTable GetTranslate() { \
        const VX::Math::vec3& v = RenderObject::GetTranslate(); \
        LuaTable t; \
        t.push(v.x); \
        t.push(v.y); \
        t.push(v.z); \
        return t; \
    } \
    LuaTable GetRotate() { \
        const VX::Math::vec3& v = RenderObject::GetRotate(); \
        LuaTable t; \
        t.push(v.x); \
        t.push(v.y); \
        t.push(v.z); \
        return t; \
    } \
    LuaTable GetScale() { \
        const VX::Math::vec3& v = RenderObject::GetScale(); \
        LuaTable t; \
        t.push(v.x); \
        t.push(v.y); \
        t.push(v.z); \
        return t; \
    } \
    LuaTable GetTransformMatrix() { \
        const VX::Math::matrix mat = RenderObject::GetTransformMatrix(); \
        LuaTable t; \
        for (int i = 0; i < 16; ++i) { \
            t.push(mat.f[i]); \
        } \
        return t; \
    } \
    LuaTable GetFloatTable() { \
        const FloatMap& m = RenderObject::GetUniformFloat(); \
        FloatMap::const_iterator it, eit = m.end(); \
        LuaTable t; \
        for (it = m.begin(); it != eit; ++it) { \
            t.map(it->first, LuaTable(it->second)); \
        } \
        return t; \
    } \
    LuaTable GetIntTable() { \
        const IntMap& m = RenderObject::GetUniformInt(); \
        IntMap::const_iterator it, eit = m.end(); \
        LuaTable t; \
        for (it = m.begin(); it != eit; ++it) { \
            t.map(it->first, LuaTable(it->second)); \
        } \
        return t; \
    } \
    LuaTable GetVec2Table() { \
        const Vec2Map& m = RenderObject::GetUniformVec2(); \
        Vec2Map::const_iterator it, eit = m.end(); \
        LuaTable t; \
        for (it = m.begin(); it != eit; ++it) { \
            t.map(it->first, LuaTable(it->second.x, it->second.y)); \
        } \
        return t; \
    } \
    LuaTable GetVec3Table() { \
        const Vec3Map& m = RenderObject::GetUniformVec3(); \
        Vec3Map::const_iterator it, eit = m.end(); \
        LuaTable t; \
        for (it = m.begin(); it != eit; ++it) { \
            t.map(it->first, LuaTable(it->second.x, it->second.y, it->second.z)); \
        } \
        return t; \
    } \
    LuaTable GetVec4Table() { \
        const Vec4Map& m = RenderObject::GetUniformVec4(); \
        Vec4Map::const_iterator it, eit = m.end(); \
        LuaTable t; \
        for (it = m.begin(); it != eit; ++it) { \
            t.map(it->first, LuaTable(it->second.x, it->second.y, it->second.z, it->second.w)); \
        } \
        return t; \
    } \
    LuaTable GetTextureTable() { \
        const TextureMap& m = RenderObject::GetUniformTexture(); \
        TextureMap::const_iterator it, eit = m.end(); \
        LuaTable t; \
        for (it = m.begin(); it != eit; ++it) { \
            LuaTable im; \
            if (it->second) { \
                im.map("width", it->second->Width()); \
                im.map("height", it->second->Height()); \
                LuaTable rgba; \
                if (it->second->FloatImageBuffer()) { \
                    int size = it->second->FloatImageBuffer()->GetNum(); \
                    for (int i = 0; i < size; ++i) { \
                        rgba.push(it->second->FloatImageBuffer()->GetBuffer()[i]); \
                    } \
                } else if (it->second->ImageBuffer()) { \
                    int size = it->second->ImageBuffer()->GetNum(); \
                    for (int i = 0; i < size; ++i) { \
                        rgba.push(it->second->ImageBuffer()->GetBuffer()[i]); \
                    } \
                } \
                im.map("rgba", rgba); \
                t.map(it->first, im); \
            } \
        } \
        return t; \
    } \
    float GetFloat(const char* name) { \
        const FloatMap& m = RenderObject::GetUniformFloat(); \
        FloatMap::const_iterator it = m.find(std::string(name)); \
        if (it == m.end()) { \
            return 0.0; \
        } \
        return it->second; \
    } \
    int GetInt(const char* name) { \
        const IntMap& m = RenderObject::GetUniformInt(); \
        IntMap::const_iterator it = m.find(std::string(name)); \
        if (it == m.end()) { \
            return 0; \
        } \
        return it->second; \
    } \
    LuaTable GetVec2(const char* name) { \
        const Vec2Map& m = RenderObject::GetUniformVec2(); \
        Vec2Map::const_iterator it = m.find(std::string(name)); \
        if (it == m.end()) { \
            return LuaTable(0.0, 0.0); \
        } \
        return LuaTable(it->second.x, it->second.y); \
    } \
    LuaTable GetVec3(const char* name) { \
        const Vec3Map& m = RenderObject::GetUniformVec3(); \
        Vec3Map::const_iterator it = m.find(std::string(name)); \
        if (it == m.end()) { \
            return LuaTable(0.0, 0.0, 0.0); \
        } \
        return LuaTable(it->second.x, it->second.y, it->second.z); \
    } \
    LuaTable GetVec4(const char* name) { \
        const Vec4Map& m = RenderObject::GetUniformVec4(); \
        Vec4Map::const_iterator it = m.find(std::string(name)); \
        if (it == m.end()) { \
            return LuaTable(0.0, 0.0, 0.0, 0.0); \
        } \
        return LuaTable(it->second.x, it->second.y, it->second.z, it->second.w); \
    } \
    bool SetTexture(const char* name, BufferImageData_Lua* tex) { \
        return RenderObject::SetTexture(name, tex); \
    } \
    bool SetTextureFiltering(const char* name, bool filter) { \
        return RenderObject::SetTextureFiltering(name, filter); \
    } \
    bool SetTextureWrapping(const char* name, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR) { \
        return RenderObject::SetTextureWrapping(name, clampToEdgeS, clampToEdgeT, clampToEdgeR); \
    } \
    bool AddExtraBuffer(const std::string& varyingName, BufferExtraData_Lua* data) { \
        return RenderObject::AddExtraBuffer(varyingName, data); \
    } \
    bool RemoveExtraBuffer(const std::string& varyingName) { \
        return RenderObject::RemoveExtraBuffer(varyingName); \
    } \
    bool ClearExtraBuffer() { \
        return RenderObject::ClearExtraBuffer(); \
    } \
    BufferImageData_Lua* GetTexture(const char* name) { \
        const TextureMap& m = RenderObject::GetUniformTexture(); \
        TextureMap::const_iterator it = m.find(std::string(name)); \
        if (it == m.end()) { \
            return 0; \
        } \
        /* TODO: implemantation */ \
        return 0;/*BufferImageData_Lua(it->second);*/ \
    } \
    LuaTable GetBBox() { \
        LuaTable res; \
        VX::Math::vec3 bmin(0, 0, 0); \
        VX::Math::vec3 bmax(0, 0, 0); \
        if (GetBox(bmin, bmax)) { \
            LuaTable tmin(bmin.x, bmin.y, bmin.z); \
            LuaTable tmax(bmax.x, bmax.y, bmax.z); \
            res.push(tmin); \
            res.push(tmax); \
        } \
        return res; \
    }

#define DECLARE_RENDEROBJECT_LUA_METHOD() \
    LUA_SCRIPTCLASS_METHOD_ARG0(std::string, GetType) \
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetTranslate,float, float, float) \
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetRotate,float, float, float) \
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetScale,float, float, float) \
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, SetTransformMatrix, LuaTable) \
    LUA_SCRIPTCLASS_METHOD_ARG3(bool, SetVec2,  const char*, float, float) \
    LUA_SCRIPTCLASS_METHOD_ARG4(bool, SetVec3,  const char*, float, float, float) \
    LUA_SCRIPTCLASS_METHOD_ARG5(bool, SetVec4,  const char*, float, float, float, float) \
    LUA_SCRIPTCLASS_METHOD_ARG2(bool, SetFloat, const char*, float) \
    LUA_SCRIPTCLASS_METHOD_ARG2(bool, SetInt, const char*, int) \
    LUA_SCRIPTCLASS_METHOD_ARG2(bool, SetTexture, const char*, BufferImageData_Lua*) \
    LUA_SCRIPTCLASS_METHOD_ARG2(bool, SetTextureFiltering, const char*, bool) \
    LUA_SCRIPTCLASS_METHOD_ARG4(bool, SetTextureWrapping, const char*, bool, bool, bool) \
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetTranslate) \
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetRotate) \
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetScale) \
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetTransformMatrix) \
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetVec4Table) \
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetVec3Table) \
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetVec2Table) \
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetFloatTable) \
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetTextureTable) \
    LUA_SCRIPTCLASS_METHOD_ARG1(LuaTable, GetVec4, const char*) \
    LUA_SCRIPTCLASS_METHOD_ARG1(LuaTable, GetVec3, const char*) \
    LUA_SCRIPTCLASS_METHOD_ARG1(LuaTable, GetVec2, const char*) \
    LUA_SCRIPTCLASS_METHOD_ARG1(float, GetFloat, const char*) \
    LUA_SCRIPTCLASS_METHOD_ARG1(int, GetInt, const char*) \
    LUA_SCRIPTCLASS_METHOD_ARG2(bool, AddExtraBuffer, const char*, BufferExtraData_Lua*) \
    LUA_SCRIPTCLASS_METHOD_ARG1(bool, RemoveExtraBuffer, const char*) \
    LUA_SCRIPTCLASS_METHOD_ARG0(bool, ClearExtraBuffer) \
    LUA_SCRIPTCLASS_METHOD_ARG1(BufferImageData_Lua, GetTexture, const char*) \
    LUA_SCRIPTCLASS_METHOD_ARG0(LuaTable, GetBBox)


#endif //_RENDEROBJECT_LUA_H_
