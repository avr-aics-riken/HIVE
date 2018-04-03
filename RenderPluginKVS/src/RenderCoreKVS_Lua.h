/*
    RenderPluginKVS/RenderPluginKVS_Lua.h
*/

#include "LuaUtil.h"
#include "RenderCoreKVS.h"
#include <SceneScript/RenderCore_Lua.h>

class RenderCoreKVS_Lua : public RenderCore_Lua
{
private:
    
    RenderCoreKVS_Lua()
    : RenderCore_Lua(static_cast<RenderCore*>(new RenderCoreKVS()))
    {
    }
    
    int render(LuaTable tbl, LuaTable cb) {
        RenderCoreKVS* rcore = static_cast<RenderCoreKVS*>(core);
        return rcore->render(tbl, cb);
    }
    
public:
    ~RenderCoreKVS_Lua() {}
    
    LUA_SCRIPTCLASS_BEGIN(RenderCoreKVS_Lua)
    
    /* RenderCore compatible interface */
    LUA_SCRIPTCLASS_METHOD_ARG2(int, render, LuaTable, LuaTable);
    LUA_SCRIPTCLASS_METHOD_ARG0(int, clearCache);
    LUA_SCRIPTCLASS_METHOD_ARG1(int, clearShaderCache, const char*);
    LUA_SCRIPTCLASS_METHOD_ARG0(std::string, getRendererName);

    /* RenderCoreKVS interface */
    
    // TODO:

    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(RenderCoreKVS_Lua);
