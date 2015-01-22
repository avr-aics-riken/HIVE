
#include <stdio.h>
#include <stdlib.h>

#include "SceneScript.h"

#include "LuaUtil.h"

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
    doLua(L, luascript);
    closeLua(L);
    
    delete [] luascript;
    
    return true;
}
