/*
    RenderPluginSURFACE/main.cpp
*/

#include "Commands.h"
#include "LuaUtil.h"

#include "RenderPluginSURFACE.h"
#include <Renderer/RenderCore.h>
namespace {

    void SetParallelRendering(bool enableParallel)
    {
        SetScreenParallel_SGL(enableParallel, false);
    }

    void SampleCoverage(float fsaa)
    {
        SampleCoverage_SGL(fsaa, 0);
    }

    void PixelStep(int pixelstep)
    {
        PixelStep_SGL(pixelstep);
    }

    RenderCore* core = 0;
    



    int render(lua_State* L)
    {
        const int stnum = lua_gettop(L);
        if (stnum < 1) {
            fprintf(stderr,"Invalid function args: render({RenderObjects}");
            lua_pushnumber(L, 0);
            return 1;
        }
        
        LuaTable tbl(L, 1);
        const std::vector<LuaTable>& robjs = tbl.GetTable();
        printf("RenderObjects Num = %d\n", static_cast<int>(robjs.size()));
        
        if (stnum > 1 && lua_type(L, 2) == LUA_TFUNCTION) { // progress callback function
            //g_L = L;
            //core->SetProgressCallback(progressCallback);
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
        
        lua_pushnumber(L, 1);
        return 1;
    }

    int clearCache(lua_State* L)
    {
        core->ClearBuffers();
        lua_pushnumber(L, 1);
        return 1;
    }

    int clearShaderCache(lua_State* L)
    {
        const char* src = lua_tostring(L, 1);
        core->ClearShaderCache(src);
        lua_pushnumber(L, 1);
        return 1;
    }

    int screenParallelRendering(lua_State* L)
    {
        const bool para = lua_toboolean(L, 1);
        //core->SetParallelRendering(para);
        lua_pushboolean(L, true);
        return 1;
    }
    
}

// LUA export

extern "C" {

int luaopen_RenderPluginSURFACE(lua_State* L)
{
    printf("CALL luaopen_RenderPluginSURFACE\n");
    RenderPlugin* renderPlugin = new RenderPluginSURFACE();
    core = new RenderCore(renderPlugin);
    SetFunction(L, "render", render);
    SetFunction(L, "clearCache", clearCache);
    SetFunction(L, "clearShaderCache", clearShaderCache);
    //SetFunction(L, "renderMode", renderMode);
    SetFunction(L, "screenParallelRendering", screenParallelRendering);
    return 0;
}

}
