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
protected:
    RenderCore* core;
    
public:
    RenderCore_Lua() : core(0) {}
    RenderCore_Lua(RenderCore* core_) : core(core_) {}
    ~RenderCore_Lua(){};

    std::string getRendererName() {
        return std::string(core->GetRendererName());
    }
    
    int render(LuaTable tbl, LuaTable callbackfunc) {
        
        const std::vector<LuaTable>& robjs = tbl.GetTable();
        printf("RenderObjects Num = %d\n", static_cast<int>(robjs.size()));
        
        if (callbackfunc.GetType() == LuaTable::TYPE_FUNCTION) {
            /*
                TODO: call LUA funciton callback
             */
            //core->SetProgressCallback();
        }
        
        
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
    LUA_SCRIPTCLASS_METHOD_ARG2(int, render, LuaTable, LuaTable);
    LUA_SCRIPTCLASS_METHOD_ARG0(int, clearCache);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, clearShaderCache, const char*);
    LUA_SCRIPTCLASS_METHOD_ARG0(std::string, getRendererName);
    
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(RenderCore_Lua);

#endif //_RENDERCORE_LUA_H_

