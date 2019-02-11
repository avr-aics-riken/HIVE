/*
    RenderPluginSURFACE/main.cpp
*/

#include "Renderer/Commands.h"
#include "LuaUtil.h"

#include "Renderer/RenderDevice.h"
#include "Renderer/RenderCoreSURFACE.h"
#include <SceneScript/RenderCore_Lua.h>

class RenderCoreSURFACE_Lua : public RenderCore_Lua
{
private:
    
    RenderCoreSURFACE_Lua()
    : RenderCore_Lua(static_cast<RenderCore*>(new RenderCoreSURFACE(new RenderDevice())))
    {
    }
    
    int setParallelRendering(bool enableParallel)
    {
        SetScreenParallel_SGL(enableParallel, false);
        return 0;
    }
    
    int sampleCoverage(float fsaa)
    {
        SampleCoverage_SGL(fsaa, 0);
        return 0;
    }
    
    int pixelStep(int pixelstep)
    {
        PixelStep_SGL(pixelstep);
        return 0;
    }
    
    int enableDepthComposition(bool enable) {
        RenderCoreSURFACE* surface = reinterpret_cast<RenderCoreSURFACE*>(core);
        if (surface) {
            surface->EnableDepthComposition(enable);
        }
        return 0;
    }

    int setCompositionMergeID(int mergeID) {
        RenderCoreSURFACE* surface = reinterpret_cast<RenderCoreSURFACE*>(core);
        if (surface) {
            surface->SetCompositionMergeID(mergeID);
        }
        return 0;
    }
    
public:
    //RenderCoreSURFACE_Lua(RenderCoreSURFACE* core) : RenderCore_Lua(static_cast<RenderCore*>(core)){}
    ~RenderCoreSURFACE_Lua() {}
    
    LUA_SCRIPTCLASS_BEGIN(RenderCoreSURFACE_Lua)
    
    /* RenderCore compatible interface */
    LUA_SCRIPTCLASS_METHOD_ARG2(int, render, LuaTable, LuaTable);
    LUA_SCRIPTCLASS_METHOD_ARG0(int, clearCache);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, clearShaderCache, const char*);
    LUA_SCRIPTCLASS_METHOD_ARG0(std::string, getRendererName);

    /* RenderCoreSURFACE interface */
    LUA_SCRIPTCLASS_METHOD_ARG1(int, setParallelRendering, bool);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, sampleCoverage, float);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, pixelStep, int);

    LUA_SCRIPTCLASS_METHOD_ARG1(int, enableDepthComposition, bool);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, setCompositionMergeID, int);

    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(RenderCoreSURFACE_Lua);

// LUA export

extern "C" {

int luaopen_RenderPluginSURFACE(lua_State* L)
{
    
    LUA_SCRIPTCLASS_REGISTER(L, RenderCoreSURFACE_Lua);
    lua_pushcfunction(L, LUA_SCRIPTCLASS_NEW_FUNCTION(RenderCoreSURFACE_Lua));
    
    /*
    printf("CALL luaopen_RenderPluginSURFACE\n");
     
    // make static instance
    if (core) {
      LUAPUSH<RenderCoreSURFACE_Lua*>(L, new RenderCoreSURFACE_Lua(core));
      return 1;
    }
    
    RenderDevice* renderPlugin = new RenderDevice();
    core = new RenderCoreSURFACE(renderPlugin);
    
    LUAPUSH<RenderCoreSURFACE_Lua*>(L, new RenderCoreSURFACE_Lua(core));*/
    return 1;
}

}
