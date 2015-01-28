
#include <stdio.h>
#include <stdlib.h>

#include "SceneScript.h"

#include "LuaUtil.h"

#include <vector>

#include "../Renderer/RenderCore.h"

// --- Script Classes ----
#include "PolygonModel_Lua.h"
#include "VolumeModel_Lua.h"
#include "Camera_Lua.h"
#include "BufferMeshData_Lua.h"
#include "BufferVolumeData_Lua.h"
#include "ObjLoader_Lua.h"
#include "StlLoader_Lua.h"
#include "VolLoader_Lua.h"
#include "SphLoader_Lua.h"
#include "PolygonAnalyzer_Lua.h"
#include "FloatsToFloat_Lua.h"

void RegisterSceneClass(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, PolygonModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, VolumeModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, Camera_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferMeshData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferVolumeData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, OBJLoader_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, STLLoader_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, VOLLoader_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, SPHLoader_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, PolygonAnalyzer_Lua);
    SetFunction(L, "PolygonModel",        LUA_SCRIPTCLASS_NEW_FUNCTION(PolygonModel_Lua));
    SetFunction(L, "VolumeModel",         LUA_SCRIPTCLASS_NEW_FUNCTION(VolumeModel_Lua));
    SetFunction(L, "Camera",              LUA_SCRIPTCLASS_NEW_FUNCTION(Camera_Lua));
    SetFunction(L, "MeshData",            LUA_SCRIPTCLASS_NEW_FUNCTION(BufferMeshData_Lua));
    SetFunction(L, "VolumeData",          LUA_SCRIPTCLASS_NEW_FUNCTION(BufferVolumeData_Lua));
    SetFunction(L, "OBJLoader",           LUA_SCRIPTCLASS_NEW_FUNCTION(OBJLoader_Lua));
    SetFunction(L, "STLLoader",           LUA_SCRIPTCLASS_NEW_FUNCTION(STLLoader_Lua));
    SetFunction(L, "VOLLoader",           LUA_SCRIPTCLASS_NEW_FUNCTION(VOLLoader_Lua));
    SetFunction(L, "SPHLoader",           LUA_SCRIPTCLASS_NEW_FUNCTION(SPHLoader_Lua));
    SetFunction(L, "PolygonAnalyzer",     LUA_SCRIPTCLASS_NEW_FUNCTION(PolygonAnalyzer_Lua));
    SetFunction(L, "FloatsToFloat",       LUA_SCRIPTCLASS_NEW_FUNCTION(FloatsToFloat_Lua));
}
// ------------------------

int getRenderObjectFromTable(lua_State* L, int n, std::vector<RenderObject*>& robjs)
{
    lua_pushvalue(L, n); // stack on top

    if (!lua_istable(L, -1))
        return 0;
    int num = 0;
    lua_pushnil(L);
    while(lua_next(L, -2) != 0){
        ++num;
        PolygonModel_Lua* robj = LUACAST<PolygonModel_Lua*>(L, -1); // val
        robjs.push_back(robj);
        lua_pop(L, 1);
    }
    return num;
}

int render(lua_State* L)
{
    const int stnum = lua_gettop(L);
    if (stnum < 1) {
        printf("Invalid function args: render({RenderObjects}");
        lua_pushnumber(L, 0);
        return 1;
    }
    
    // register models
    std::vector<RenderObject*> robjs;
    const int modelnum = getRenderObjectFromTable(L, 1, robjs);
    printf("RenderObjects Num = %d\n", modelnum);
 
    RenderCore* core = RenderCore::GetInstance();
    
    for (size_t i = 0; i < robjs.size(); ++i) {
        core->AddRenderObject(robjs[i]);
    }
    
    // call render
    core->Render();
               
    // clear
    core->ClearRenderObject();
    
    lua_pushnumber(L, 1);
    return 1;
}

void registerFuncs(lua_State* L)
{
    SetFunction(L, "render", render);
    RegisterSceneClass(L);
}

bool SceneScript::Execute(const char* scenefile)
{
    printf("Execute Scene file:%s\n", scenefile);
    
    FILE* fp = fopen(scenefile, "rb");
    if (!fp)
        return false;
    
    fseek(fp, 0, SEEK_END);
    const int scriptsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char* luascript = new char[scriptsize + 1];
    fread(luascript, scriptsize, 1, fp);
    luascript[scriptsize] = 0; // END
    
    lua_State* L = createLua();
    registerFuncs(L);
    doLua(L, luascript);
    closeLua(L);
    
    delete [] luascript;
    
    return true;
}
