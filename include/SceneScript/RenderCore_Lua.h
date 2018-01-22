/**
 * @file RenderCore_Lua.h
 * STLLoader Luaラッパー
 */
#ifndef _RENDERCORE_LUA_H_
#define _RENDERCORE_LUA_H_


#include "LuaUtil.h"
#include "../Renderer/RenderCore.h"

class RenderObject_Lua;
/**
 * RenderCore Luaラッパー
 */
class RenderCore_Lua : public RefCount
{
private:
    RenderCore* core;
    
public:
    RenderCore_Lua() : core(0) {}
    RenderCore_Lua(RenderCore* core_) : core(core_) {}
    ~RenderCore_Lua(){};

    int render(LuaTable tbl) {
        
        const int stnum = 0;//lua_gettop(L); // TODO
        /*if (stnum < 1) {
            fprintf(stderr,"Invalid function args: render({RenderObjects}");
            lua_pushnumber(L, 0);
            return 1;
        }*/
        
        //LuaTable tbl(L, 1);
        const std::vector<LuaTable>& robjs = tbl.GetTable();
        printf("RenderObjects Num = %d\n", static_cast<int>(robjs.size()));
        
        //if (stnum > 1 && lua_type(L, 2) == LUA_TFUNCTION) { // progress callback function
            //RenderObject::SetProgressCallback(progressCallback);
        //}
        
        
        for (size_t i = 0; i < robjs.size(); ++i) {
            LuaRefPtr<RenderObject>* ro = robjs[i].GetUserData<RenderObject>();
            if (!ro)
                continue;
            core->AddRenderObject(*ro);
        }
        
        // call render
        core->Render();
        
        // clear
        core->ClearRenderObject();
        
        //lua_pushnumber(L, 1);
        return 1;
    }
    int clearCache() {
        core->ClearBuffers();
        return 0;
    }
    int clearShaderCache(const char* src) {
        core->ClearShaderCache(src);
        return 0;
    }
    
    LUA_SCRIPTCLASS_BEGIN(RenderCore_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(int, render, LuaTable);
    LUA_SCRIPTCLASS_METHOD_ARG0(int, clearCache);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, clearShaderCache, const char*);
    
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(RenderCore_Lua);

#endif //_RENDERCORE_LUA_H_

