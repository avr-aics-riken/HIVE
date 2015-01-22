
#include <stdio.h>
#include <stdlib.h>

#include "SceneScript.h"

#include "LuaUtil.h"

#include <vector>

#include "../RenderObject/RenderObject.h"

#include "../Renderer/RenderCore.h"


int getRenderObjectFromTable(lua_State* L, int n, std::vector<RenderObject*>& robjs)
{
    if (!lua_istable(L, -1))
        return 0;
    lua_pushnil(L);
    int num = 0;
    while(lua_next(L, -2) != 0){
        ++num;
        
        std::string feildname = static_cast<std::string>(lua_tostring(L, -2)); // name
        RenderObject* robj = LUACAST<RenderObject*>(L, -1); // val
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
    //RegisterSceneClass(L); // ここでクラス群を登録
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
