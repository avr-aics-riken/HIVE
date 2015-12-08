#ifdef HIVE_ENABLE_MPI
#include <mpi.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "SceneScript.h"

#include "LuaUtil.h"

#include <vector>

#include "../Renderer/RenderCore.h"
/*
// --- Script Classes ----
#include "PolygonModel_Lua.h"
#include "VolumeModel_Lua.h"
#include "SparseVolumeModel_Lua.h"
#include "PointModel_Lua.h"
#include "LineModel_Lua.h"
#include "VectorModel_Lua.h"
#include "TetraModel_Lua.h"
#include "Camera_Lua.h"
#include "MetaBinary_Lua.h"
#include "BufferMeshData_Lua.h"
#include "BufferLineData_Lua.h"
#include "BufferVolumeData_Lua.h"
#include "BufferSparseVolumeData_Lua.h"
#include "BufferPointData_Lua.h"
#include "BufferPointData_Lua.h"
#include "BufferImageData_Lua.h"
#include "BufferTetraData_Lua.h"
#include "BufferVectorData_Lua.h"
#include "ImageLoader_Lua.h"
#include "ImageSaver_Lua.h"
#include "GenTexture_Lua.h"

void RegisterSceneClass(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, PolygonModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, VolumeModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, SparseVolumeModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, PointModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, LineModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, VectorModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, TetraModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, Camera_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, MetaBinary_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferMeshData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferLineData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferVolumeData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferSparseVolumeData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferPointData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferImageData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferTetraData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferVectorData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, ImageLoader_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, ImageSaver_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, GenTexture_Lua);
    SetFunction(L, "PolygonModel",        LUA_SCRIPTCLASS_NEW_FUNCTION(PolygonModel_Lua));
    SetFunction(L, "VolumeModel",         LUA_SCRIPTCLASS_NEW_FUNCTION(VolumeModel_Lua));
    SetFunction(L, "SparseVolumeModel",   LUA_SCRIPTCLASS_NEW_FUNCTION(SparseVolumeModel_Lua));
    SetFunction(L, "PointModel",          LUA_SCRIPTCLASS_NEW_FUNCTION(PointModel_Lua));
    SetFunction(L, "LineModel",           LUA_SCRIPTCLASS_NEW_FUNCTION(LineModel_Lua));
    SetFunction(L, "VectorModel",         LUA_SCRIPTCLASS_NEW_FUNCTION(VectorModel_Lua));
    SetFunction(L, "TetraModel",          LUA_SCRIPTCLASS_NEW_FUNCTION(TetraModel_Lua));
    SetFunction(L, "Camera",              LUA_SCRIPTCLASS_NEW_FUNCTION(Camera_Lua));
    SetFunction(L, "MetaBinary",          LUA_SCRIPTCLASS_NEW_FUNCTION(MetaBinary_Lua));
    SetFunction(L, "MeshData",            LUA_SCRIPTCLASS_NEW_FUNCTION(BufferMeshData_Lua));
    SetFunction(L, "LineData",            LUA_SCRIPTCLASS_NEW_FUNCTION(BufferLineData_Lua));
    SetFunction(L, "VolumeData",          LUA_SCRIPTCLASS_NEW_FUNCTION(BufferVolumeData_Lua));
    SetFunction(L, "SparseVolumeData",    LUA_SCRIPTCLASS_NEW_FUNCTION(BufferSparseVolumeData_Lua));
    SetFunction(L, "PointData",           LUA_SCRIPTCLASS_NEW_FUNCTION(BufferPointData_Lua));
    SetFunction(L, "ImageData",           LUA_SCRIPTCLASS_NEW_FUNCTION(BufferImageData_Lua));
    SetFunction(L, "TetraData",           LUA_SCRIPTCLASS_NEW_FUNCTION(BufferTetraData_Lua));
    SetFunction(L, "VectorData",          LUA_SCRIPTCLASS_NEW_FUNCTION(BufferVectorData_Lua));
    SetFunction(L, "ImageLoader",         LUA_SCRIPTCLASS_NEW_FUNCTION(ImageLoader_Lua));
    SetFunction(L, "ImageSaver",          LUA_SCRIPTCLASS_NEW_FUNCTION(ImageSaver_Lua));
    SetFunction(L, "GenTexture",          LUA_SCRIPTCLASS_NEW_FUNCTION(GenTexture_Lua));
}
*/
// ------------------------

/*
    Util Functions
 */
int mpiMode(lua_State* L)
{
#ifdef HIVE_ENABLE_MPI
    lua_pushboolean(L, 1);
#else
    lua_pushboolean(L, 0);
#endif
    return 1;
}

int mpiRank(lua_State* L)
{
    int rank = 0;
#ifdef HIVE_ENABLE_MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif
    lua_pushnumber(L, rank);
    return 1;
}

int mpiSize(lua_State* L)
{
    int size = 1;
#ifdef HIVE_ENABLE_MPI
    MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif
    lua_pushnumber(L, size);
    return 1;
}

int platform(lua_State* L)
{
#if _WIN32
    lua_pushstring(L, "Windows");
#elif __APPLE__
    lua_pushstring(L, "MacOSX");
#else
    lua_pushstring(L, "Linux");
#endif
    return 1;
}

int dllExtension(lua_State* L)
{
#if _WIN32
    lua_pushstring(L, "dll");
#else
    lua_pushstring(L, "so");
#endif
    return 1;
}

int endian(lua_State* L)
{
    int x=1; // 0x00000001
    if (*(char*)&x) {
        /* little endian. memory image 01 00 00 00 */
        lua_pushstring(L, "little");
    }else{
        /* big endian. memory image 00 00 00 01 */
        lua_pushstring(L, "big");
    }
    return 1;
}


// ------------------------
/*int getRenderObjectFromTable(lua_State* L, int n, std::vector<RenderObject*>& robjs)
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
}*/

static lua_State* g_L = 0;
static bool progressCallback(double val)
{
    lua_pushvalue(g_L, 2); // funciton
    lua_pushnumber(g_L, val); // arg
    //dumpStack(g_L);
    if (lua_pcall(g_L, 1, 1, 0)) { // arg:1, ret:1
        // error
        fprintf(stderr,"Invalid call renderer callback function: %s\n", lua_tostring(g_L, -1));
    } else {
        const bool r = lua_toboolean(g_L, -1); // ret
        return r;
    }
    lua_pop(g_L, 1);  // pop returned value
    return true; // true:continue, false: exit
}

int render(lua_State* L)
{
    const int stnum = lua_gettop(L);
    if (stnum < 1) {
        fprintf(stderr,"Invalid function args: render({RenderObjects}");
        lua_pushnumber(L, 0);
        return 1;
    }
    
    // register models
    //std::vector<RenderObject*> robjs;
    //const int modelnum = getRenderObjectFromTable(L, 1, robjs);
    //printf("RenderObjects Num = %d\n", modelnum);
    
    LuaTable tbl(L, 1);
    const std::vector<LuaTable>& robjs = tbl.GetTable();
    printf("RenderObjects Num = %d\n", static_cast<int>(robjs.size()));
    
    RenderCore* core = RenderCore::GetInstance();

    if (stnum > 1 && lua_type(L, 2) == LUA_TFUNCTION) { // progress callback function
        g_L = L;
        core->SetProgressCallback(progressCallback);
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
    RenderCore* core = RenderCore::GetInstance();
    core->ClearBuffers();
    lua_pushnumber(L, 1);
    return 1;
}

int screenParallelRendering(lua_State* L)
{
    RenderCore* core = RenderCore::GetInstance();
    const bool para = lua_toboolean(L, 1);
    core->SetParallelRendering(para);
    lua_pushboolean(L, true);
    return 1;
}

void registerFuncs(lua_State* L)
{
    SetFunction(L, "render", render);
    SetFunction(L, "clearCache", clearCache);

    SetFunction(L, "mpiMode", mpiMode);
    SetFunction(L, "mpiRank", mpiRank);
    SetFunction(L, "mpiSize", mpiSize);
    SetFunction(L, "platform", platform);
    SetFunction(L, "dllExtension", dllExtension);
    SetFunction(L, "endian", endian);
    
    SetFunction(L, "screenParallelRendering", screenParallelRendering);

    //RegisterSceneClass(L);
}

void registerArg(lua_State* L, const std::vector<std::string>& sceneargs)
{
    LuaTable arg;
    
    for (int i = 0, size = static_cast<int>(sceneargs.size()); i < size; ++i) {
        arg.push(sceneargs[i]);
    }
    
    arg.pushLuaTableValue(L);
    lua_setglobal(L, "arg");
    //dumpStack(L);
}

bool SceneScript::Execute(const char* scenefile, const std::vector<std::string>& sceneargs)
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
    if (!sceneargs.empty()) {
        registerArg(L, sceneargs);
    }
    doLua(L, luascript);
    closeLua(L);
    
    delete [] luascript;
    
    return true;
}
