#ifdef HIVE_ENABLE_MPI
#include <mpi.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "SceneScript.h"

#include "LuaUtil.h"
#include "Path.h"
#include "sleep.h"

#include <vector>
#include <algorithm>

#include "../Renderer/RenderCore.h"
#include "../Buffer/UserBufferData.h"

#include "Core/Perf.h"

#define CPP_IMPL_INSTANCE
#include "BufferMeshData_Lua.h"
#include "BufferLineData_Lua.h"
#include "BufferVolumeData_Lua.h"
#include "BufferSparseVolumeData_Lua.h"
#include "BufferPointData_Lua.h"
#include "BufferPointData_Lua.h"
#include "BufferImageData_Lua.h"
#include "BufferTetraData_Lua.h"
#include "BufferVectorData_Lua.h"
#include "BufferExtraData_Lua.h"
#undef CPP_IMPL_INSTANCE

// --- Script Classes ----
#include "PolygonModel_Lua.h"
#include "VolumeModel_Lua.h"
#include "SparseVolumeModel_Lua.h"
#include "PointModel_Lua.h"
#include "LineModel_Lua.h"
#include "VectorModel_Lua.h"
#include "TetraModel_Lua.h"
#include "SolidModel_Lua.h"
#include "Camera_Lua.h"
#include "ImageLoader_Lua.h"
#include "ImageSaver_Lua.h"
#include "GenTexture_Lua.h"
#include "PrimitiveGenerator_Lua.h"

#ifdef HIVE_STATIC
    #include "Network_Lua.h"
    #include "ObjLoader_Lua.h"
    #include "StlLoader_Lua.h"
    #include "VolLoader_Lua.h"
    #include "RawVolumeLoader_Lua.h"
    #include "RawVolumeSaver_Lua.h"
    #include "SphLoader_Lua.h"
    #include "SphSaver_Lua.h"
    #include "VtkLoader_Lua.h"
    #include "MeshAttribToVolume_Lua.h"
    #ifdef HIVE_WITH_CDMLIB
        #include "CdmLoader_Lua.h"
    #endif
    #ifdef HIVE_WITH_PDMLIB
        #include "PdmLoader_Lua.h"
        #include "PdmSaver_Lua.h"
    #endif
    #ifdef HIVE_WITH_HDMLIB
        #include "HdmLoader_Lua.h"
    #endif
    #ifdef HIVE_WITH_UDMLIB
        #include "UdmLoader_Lua.h"
    #endif
    #ifdef HIVE_WITH_BCMTOOLS
        #include "BcmLoader_Lua.h"
    #endif
    #ifdef HIVE_WITH_TPUTIL
        #include "TpUtil_Lua.h"
    #endif
    #include "PdbLoader_Lua.h"
    #include "Analyzer_Lua.h"
    #include "FloatsToFloat_Lua.h"
    #include "PointToVolume_Lua.h"
    #include "VolumeToVector_Lua.h"
    #include "VolumeToMeshData_Lua.h"
    #include "VolumeFilter_Lua.h"
    #include "VolumeQuantizer_Lua.h"
    #include "SparseVolumeToVolume_Lua.h"

    #include "ClusterParallelCoord_Lua.h"
    #include "ImageFilter_Lua.h"
    #include "MandelbulbVolGen_Lua.h"
    #include "RawSparseVolumeLoader_Lua.h"
    #include "SolidAttribToVolume_Lua.h"
    #include "SolidDfToVolume_Lua.h"
    #include "SolidToVolume_Lua.h"
    #include "VisioColorMapLoader_Lua.h"
    #include "VisioColorMapSaver_Lua.h"
    #include "VisioGradientMapLoader_Lua.h"
    #include "VisioGradientMapSaver_Lua.h"
#endif

void RegisterSceneClass(lua_State* L)
{
    LUA_SCRIPTCLASS_REGISTER(L, PolygonModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, VolumeModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, SparseVolumeModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, PointModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, LineModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, VectorModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, TetraModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, SolidModel_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, Camera_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferMeshData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferLineData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferVolumeData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferSparseVolumeData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferPointData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferImageData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferTetraData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, BufferVectorData_Lua);
    LUA_SCRIPTCLASS_REGISTER(L, PrimitiveGenerator_Lua);
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
    SetFunction(L, "SolidModel",          LUA_SCRIPTCLASS_NEW_FUNCTION(SolidModel_Lua));
    SetFunction(L, "Camera",              LUA_SCRIPTCLASS_NEW_FUNCTION(Camera_Lua));
    SetFunction(L, "MeshData",            LUA_SCRIPTCLASS_NEW_FUNCTION(BufferMeshData_Lua));
    SetFunction(L, "LineData",            LUA_SCRIPTCLASS_NEW_FUNCTION(BufferLineData_Lua));
    SetFunction(L, "VolumeData",          LUA_SCRIPTCLASS_NEW_FUNCTION(BufferVolumeData_Lua));
    SetFunction(L, "SparseVolumeData",    LUA_SCRIPTCLASS_NEW_FUNCTION(BufferSparseVolumeData_Lua));
    SetFunction(L, "PointData",           LUA_SCRIPTCLASS_NEW_FUNCTION(BufferPointData_Lua));
    SetFunction(L, "ImageData",           LUA_SCRIPTCLASS_NEW_FUNCTION(BufferImageData_Lua));
    SetFunction(L, "TetraData",           LUA_SCRIPTCLASS_NEW_FUNCTION(BufferTetraData_Lua));
    SetFunction(L, "VectorData",          LUA_SCRIPTCLASS_NEW_FUNCTION(BufferVectorData_Lua));
    SetFunction(L, "PrimitiveGenerator",  LUA_SCRIPTCLASS_NEW_FUNCTION(PrimitiveGenerator_Lua));
    SetFunction(L, "ImageLoader",         LUA_SCRIPTCLASS_NEW_FUNCTION(ImageLoader_Lua));
    SetFunction(L, "ImageSaver",          LUA_SCRIPTCLASS_NEW_FUNCTION(ImageSaver_Lua));
    SetFunction(L, "GenTexture",          LUA_SCRIPTCLASS_NEW_FUNCTION(GenTexture_Lua));
    
#ifdef HIVE_STATIC
        LUA_SCRIPTCLASS_REGISTER(L, OBJLoader_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, STLLoader_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, VOLLoader_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, RawVolumeLoader_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, RawVolumeSaver_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, SPHLoader_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, SPHSaver_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, VTKLoader_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, MeshAttribToVolume_Lua);
    #ifdef HIVE_WITH_CDMLIB
        LUA_SCRIPTCLASS_REGISTER(L, CDMLoader_Lua);
    #endif
    #ifdef HIVE_WITH_PDMLIB
        LUA_SCRIPTCLASS_REGISTER(L, PDMLoader_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, PDMSaver_Lua);
    #endif
    #ifdef HIVE_WITH_HDMLIB
        LUA_SCRIPTCLASS_REGISTER(L, HDMLoader_Lua);
    #endif
    #ifdef HIVE_WITH_UDMLIB
        LUA_SCRIPTCLASS_REGISTER(L, UDMLoader_Lua);
    #endif
    #ifdef HIVE_WITH_BCMTOOLS
        LUA_SCRIPTCLASS_REGISTER(L, BCMLoader_Lua);
    #endif
    #ifdef HIVE_WITH_TPUTIL
        LUA_SCRIPTCLASS_REGISTER(L, TPUtil_Lua);
    #endif
        LUA_SCRIPTCLASS_REGISTER(L, PDBLoader_Lua);
        
        LUA_SCRIPTCLASS_REGISTER(L, FloatsToFloat_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, PointToVolume_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, VolumeToVector_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, VolumeToMeshData_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, VolumeFilter_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, VolumeQuantizer_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, SparseVolumeToVolume_Lua);
        
        LUA_SCRIPTCLASS_REGISTER(L, ImageFilter_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, MandelbulbVolGen_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, RawSparseVolumeLoader_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, SolidAttribToVolume_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, SolidDfToVolume_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, SolidToVolume_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, VisioColorMapLoader_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, VisioColorMapSaver_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, VisioGradientMapLoader_Lua);
        LUA_SCRIPTCLASS_REGISTER(L, VisioGradientMapSaver_Lua);
        
        SetFunction(L, "ObjLoader",           LUA_SCRIPTCLASS_NEW_FUNCTION(OBJLoader_Lua));
        SetFunction(L, "StlLoader",           LUA_SCRIPTCLASS_NEW_FUNCTION(STLLoader_Lua));
        
        SetFunction(L, "VolLoader",   LUA_SCRIPTCLASS_NEW_FUNCTION(VOLLoader_Lua));
        SetFunction(L, "RawVolumeLoader",     LUA_SCRIPTCLASS_NEW_FUNCTION(RawVolumeLoader_Lua));
        SetFunction(L, "RawVolumeSaver",      LUA_SCRIPTCLASS_NEW_FUNCTION(RawVolumeSaver_Lua));
        SetFunction(L, "SphLoader",           LUA_SCRIPTCLASS_NEW_FUNCTION(SPHLoader_Lua));
        SetFunction(L, "SphSaver",            LUA_SCRIPTCLASS_NEW_FUNCTION(SPHSaver_Lua));
        SetFunction(L, "VtkLoader",           LUA_SCRIPTCLASS_NEW_FUNCTION(VTKLoader_Lua));
        SetFunction(L, "MeshAttribToVolume",  LUA_SCRIPTCLASS_NEW_FUNCTION(MeshAttribToVolume_Lua));
    #ifdef HIVE_WITH_CDMLIB
        SetFunction(L, "CdmLoader",           LUA_SCRIPTCLASS_NEW_FUNCTION(CDMLoader_Lua));
    #endif
    #ifdef HIVE_WITH_PDMLIB
        SetFunction(L, "PdmLoader",           LUA_SCRIPTCLASS_NEW_FUNCTION(PDMLoader_Lua));
        SetFunction(L, "PdmSaver",            LUA_SCRIPTCLASS_NEW_FUNCTION(PDMSaver_Lua));
    #endif
    #ifdef HIVE_WITH_HDMLIB
        SetFunction(L, "HdmLoader",           LUA_SCRIPTCLASS_NEW_FUNCTION(HDMLoader_Lua));
    #endif
    #ifdef HIVE_WITH_UDMLIB
        SetFunction(L, "UdmLoader",           LUA_SCRIPTCLASS_NEW_FUNCTION(UDMLoader_Lua));
    #endif
    #ifdef HIVE_WITH_BCMTOOLS
        SetFunction(L, "BcmLoader",           LUA_SCRIPTCLASS_NEW_FUNCTION(BCMLoader_Lua));
    #endif
    #ifdef HIVE_WITH_TPUTIL
        SetFunction(L, "TpUtil",               LUA_SCRIPTCLASS_NEW_FUNCTION(TPUtil_Lua));
    #endif
        SetFunction(L, "PdbLoader",           LUA_SCRIPTCLASS_NEW_FUNCTION(PDBLoader_Lua));
        SetFunction(L, "FloatsToFloat",       LUA_SCRIPTCLASS_NEW_FUNCTION(FloatsToFloat_Lua));
        SetFunction(L, "PointToVolume",       LUA_SCRIPTCLASS_NEW_FUNCTION(PointToVolume_Lua));
        SetFunction(L, "VolumeToVector",      LUA_SCRIPTCLASS_NEW_FUNCTION(VolumeToVector_Lua));
        SetFunction(L, "VolumeToMeshData",    LUA_SCRIPTCLASS_NEW_FUNCTION(VolumeToMeshData_Lua));
        SetFunction(L, "VolumeFilter",        LUA_SCRIPTCLASS_NEW_FUNCTION(VolumeFilter_Lua));
        SetFunction(L, "VolumeQuantizer",        LUA_SCRIPTCLASS_NEW_FUNCTION(VolumeQuantizer_Lua));
        SetFunction(L, "SparseVolumeToVolume",        LUA_SCRIPTCLASS_NEW_FUNCTION(SparseVolumeToVolume_Lua));
        
        SetFunction(L, "ImageFilter",        LUA_SCRIPTCLASS_NEW_FUNCTION(ImageFilter_Lua));
        SetFunction(L, "MandelbulbVolGen",        LUA_SCRIPTCLASS_NEW_FUNCTION(MandelbulbVolGen_Lua));
        SetFunction(L, "RawSparseVolumeLoader",        LUA_SCRIPTCLASS_NEW_FUNCTION(RawSparseVolumeLoader_Lua));
        SetFunction(L, "SolidAttribToVolume",        LUA_SCRIPTCLASS_NEW_FUNCTION(SolidAttribToVolume_Lua));
        SetFunction(L, "SolidDfToVolume",        LUA_SCRIPTCLASS_NEW_FUNCTION(SolidDfToVolume_Lua));
        SetFunction(L, "SolidToVolume",        LUA_SCRIPTCLASS_NEW_FUNCTION(SolidToVolume_Lua));
        SetFunction(L, "VisioColorMapLoader",        LUA_SCRIPTCLASS_NEW_FUNCTION(VisioColorMapLoader_Lua));
        SetFunction(L, "VisioColorMapSaver",        LUA_SCRIPTCLASS_NEW_FUNCTION(VisioColorMapSaver_Lua));
        SetFunction(L, "VisioGradientMapLoader",        LUA_SCRIPTCLASS_NEW_FUNCTION(VisioGradientMapLoader_Lua));
        SetFunction(L, "VisioGradientMapSaver",        LUA_SCRIPTCLASS_NEW_FUNCTION(VisioGradientMapSaver_Lua));
        
        // nested modules
        register_Network_Lua(L);                // Network_Lua.h
        register_ClusterParallelCoord_Lua(L);   // ClusterParallelCoord_Lua.h
        register_Analyzer_Lua(L);               // Analyzer_Lua.h
        
#endif // HIVE_STATIC
    
}

// ------------------------

namespace {

void RegisterHiveCoreProfilingPoints()
{
    // PMon::Initialize() should be called before calling this this function.
    // (Usually outside of HiveCore modules, e.g. main())
    PMon::Register("HiveCore::Render", PMon::PMON_CALC, /* exclusive */true);
    PMon::Register("HiveCore::ImageSave", PMon::PMON_CALC, /* exclusive */false);
    PMon::Register("Compositor", PMon::PMON_CALC, /* exclusive */false);
}

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
#elif __APPLE__
    lua_pushstring(L, "dylib");
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


int h_sleep(lua_State* L)
{
    int t = lua_tointeger(L, 1);
    os_sleep(t);
    return 0;
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

int renderMode(lua_State* L)
{
    if (lua_isstring(L, -1)) {
        std::string modestr(lua_tostring(L, -1));
        if (modestr == std::string("OpenGL")) {
            RenderCore::GetInstance(RENDER_OPENGL);
        } else if (modestr == std::string("Surface")) {
            RenderCore::GetInstance(RENDER_SURFACE);
        }
    }
    return 0;
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
    //printf("RenderObjects Num = %d\n", static_cast<int>(robjs.size()));
    
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

int clearShaderCache(lua_State* L)
{
    RenderCore* core = RenderCore::GetInstance();
    const char* src = lua_tostring(L, 1);
    core->ClearShaderCache(src);
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

int getMemoryData(lua_State* L);
int getMemoryDataNames(lua_State* L);

int setBufferData(lua_State* L);
int getBufferData(lua_State* L);
int getBufferDataNames(lua_State* L);
int deleteBufferData(lua_State* L);
int clearBufferData(lua_State* L);

void registerFuncs(lua_State* L, void* sceneScriptPtr)
{
    SetFunction(L, "render", render);
    SetFunction(L, "clearCache", clearCache);
    SetFunction(L, "clearShaderCache", clearShaderCache);
    SetFunction(L, "renderMode", renderMode);
    
    SetFunction(L, "mpiMode", mpiMode);
    SetFunction(L, "mpiRank", mpiRank);
    SetFunction(L, "mpiSize", mpiSize);
    SetFunction(L, "platform", platform);
    SetFunction(L, "dllExtension", dllExtension);
    SetFunction(L, "endian", endian);
    SetFunction(L, "sleep", h_sleep);
    
    SetFunction(L, "screenParallelRendering", screenParallelRendering);

    SetFunction(L, "getMemoryData", getMemoryData);
    SetFunction(L, "getMemoryDataNames", getMemoryDataNames);
    
    SetFunction(L, "setBufferData", setBufferData);
    SetFunction(L, "getBufferData", getBufferData);
    SetFunction(L, "getBufferDataNames", getBufferDataNames);
    SetFunction(L, "deleteBufferData", deleteBufferData);
    SetFunction(L, "clearBufferData", clearBufferData);
    
    lua_pushlightuserdata(L, sceneScriptPtr);
    lua_setglobal(L, "__sceneScript");
    
    RegisterSceneClass(L);
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
    
} // namespace

//---------------------------------------------------------------------

class SceneScript::Impl {
private:
    struct UserMemoryData{
        std::string name;
        const void* ptr;
        size_t size;
        UserMemoryData(const std::string name_, const void* ptr_, size_t size_) : name(name_), ptr(ptr_), size(size_) {}
    };
    typedef std::vector<UserMemoryData> UserMemoryDataArray;
    
public:
    bool Execute(const char* luascript, const std::vector<std::string>& sceneargs);
    bool ExecuteFile(const char* scenefile, const std::vector<std::string>& sceneargs);
    
    void Begin(const std::vector<std::string>& sceneargs);
    bool Command(const char* scenecommand);
    void End();
    
    bool CreateMemoryData(const char* dataId);
    bool IsMemoryData(const char* dataId);
    bool DeleteMemoryData(const char* dataId);
    bool DeleteAllMemoryData();
    bool SetMemoryData(const char* dataId, const void* memptr, const size_t memsize);
    size_t GetMemoryDataSize(const char* dataId);
    const void* GetMemoryDataPointer(const char* dataId);
    int GetMemoryDataNum();
    const char* GetMemoryDataId(int i);
    
    void PushMemoryData(const char* dataId); // for internal
    UserBufferData& GetUserBufferData() { return m_bufferData; }

private:
    lua_State* m_L;
    UserMemoryDataArray m_memoryData;
    UserBufferData m_bufferData;
    
    UserMemoryDataArray::iterator getUserMemoryData(const char* dataId);
};


namespace {

    int getMemoryData(lua_State* L) {
        if (!lua_isstring(L, 1)) {
            lua_pushnil(L);
            return 1;
        }
        const char* dataName = lua_tostring(L, 1);
        lua_getglobal(L, "__sceneScript");
        const void* ptr = lua_touserdata(L, -1);
        if (!ptr) {
            fprintf(stderr,"Invalid sceneScript instance\n");
            lua_pushnil(L);
            return 1;
        }
        
        SceneScript::Impl* sceneScript = reinterpret_cast<SceneScript::Impl*>(const_cast<void*>(ptr));
        sceneScript->PushMemoryData(dataName);
        return 1;
    }

    int getMemoryDataNames(lua_State* L) {
        lua_getglobal(L, "__sceneScript");
        const void* ptr = lua_touserdata(L, -1);
        if (!ptr) {
            fprintf(stderr,"Invalid sceneScript instance\n");
            lua_pushnil(L);
            return 1;
        }

        LuaTable t;
        SceneScript::Impl* sceneScript = reinterpret_cast<SceneScript::Impl*>(const_cast<void*>(ptr));
        const int n = sceneScript->GetMemoryDataNum();
        for (int i = 0; i < n; ++i) {
            t.push(sceneScript->GetMemoryDataId(i));
        }
        t.pushLuaTableValue(L);
        return 1;
    }

    int setBufferData(lua_State* L) {
        dumpStack(L);
        if (!lua_isstring(L, 1)) {
            lua_pushnil(L);
            return 1;
        }
        const char* dataName = lua_tostring(L, 1);
        lua_getglobal(L, "__sceneScript");
        void* ptr = lua_touserdata(L, -1);
        SceneScript::Impl* sceneScript = reinterpret_cast<SceneScript::Impl*>(ptr);
        
        void* bufferPtr = lua_touserdata(L, 2);
        LuaRefPtr<BufferData>* buffer = *static_cast<LuaRefPtr<BufferData>**>(bufferPtr);
        
        if (sceneScript && buffer) {
            sceneScript->GetUserBufferData().SetBufferData(dataName, *buffer);
        }
        return 1;
    }
    
    int getBufferData(lua_State* L) {
        dumpStack(L);
        if (!lua_isstring(L, 1)) {
            lua_pushnil(L);
            return 1;
        }
        const char* dataName = lua_tostring(L, 1);
        lua_getglobal(L, "__sceneScript");
        void* ptr = lua_touserdata(L, -1);
        SceneScript::Impl* sceneScript = reinterpret_cast<SceneScript::Impl*>(ptr);
        
        if (sceneScript) {
            BufferData* data = sceneScript->GetUserBufferData().GetBufferData(dataName);
            if (data) {
                if (data->GetType() == BufferData::TYPE_IMAGE) {
                    BufferImageData_Lua* instance = static_cast<BufferImageData_Lua*>(data);
                    if (instance) {
                        return LUAPUSH<BufferImageData_Lua*>(L, instance);
                    }
                } else if (data->GetType() == BufferData::TYPE_MESH) {
                    BufferMeshData_Lua* instance = static_cast<BufferMeshData_Lua*>(data);
                    if (instance) {
                        return LUAPUSH<BufferMeshData_Lua*>(L, instance);
                    }
                } else if (data->GetType() == BufferData::TYPE_POINT) {
                    BufferPointData_Lua* instance = static_cast<BufferPointData_Lua*>(data);
                    if (instance) {
                        return LUAPUSH<BufferPointData_Lua*>(L, instance);
                    }
                } else if (data->GetType() == BufferData::TYPE_LINE) {
                    BufferLineData_Lua* instance = static_cast<BufferLineData_Lua*>(data);
                    if (instance) {
                        return LUAPUSH<BufferLineData_Lua*>(L, instance);
                    }
                } else if (data->GetType() == BufferData::TYPE_VECTOR) {
                    BufferVectorData_Lua* instance = static_cast<BufferVectorData_Lua*>(data);
                    if (instance) {
                        return LUAPUSH<BufferVectorData_Lua*>(L, instance);
                    }
                } else if (data->GetType() == BufferData::TYPE_TETRA) {
                    BufferTetraData_Lua* instance = static_cast<BufferTetraData_Lua*>(data);
                    if (instance) {
                        return LUAPUSH<BufferTetraData_Lua*>(L, instance);
                    }
                } else if (data->GetType() == BufferData::TYPE_SOLID) {
                    BufferSolidData_Lua* instance = static_cast<BufferSolidData_Lua*>(data);
                    if (instance) {
                        return LUAPUSH<BufferSolidData_Lua*>(L, instance);
                    }
                } else if (data->GetType() == BufferData::TYPE_VOLUME) {
                    BufferVolumeData_Lua* instance = static_cast<BufferVolumeData_Lua*>(data);
                    if (instance) {
                        return LUAPUSH<BufferVolumeData_Lua*>(L, instance);
                    }
                } else if (data->GetType() == BufferData::TYPE_SPARSEVOLUME) {
                    BufferSparseVolumeData_Lua* instance = static_cast<BufferSparseVolumeData_Lua*>(data);
                    if (instance) {
                        return LUAPUSH<BufferSparseVolumeData_Lua*>(L, instance);
                    }
                } else if (data->GetType() == BufferData::TYPE_IMAGE) {
                    BufferImageData_Lua* instance = static_cast<BufferImageData_Lua*>(data);
                    if (instance) {
                        return LUAPUSH<BufferImageData_Lua*>(L, instance);
                    }
                } else {
                    lua_pushnil(L);
                    return 1;
                }
            } else {
                lua_pushnil(L);
                return 1;
            }
        }
        return 1;
    }
    
    int deleteBufferData(lua_State* L) {
        dumpStack(L);
        if (!lua_isstring(L, 1)) {
            lua_pushnil(L);
            return 1;
        }
        const char* dataName = lua_tostring(L, 1);
        lua_getglobal(L, "__sceneScript");
        void* ptr = lua_touserdata(L, -1);
        SceneScript::Impl* sceneScript = reinterpret_cast<SceneScript::Impl*>(ptr);
        bool result = sceneScript->GetUserBufferData().DeleteBufferData(dataName);
        
        lua_pushboolean(L,result);
        return 1;
    }
    
    int clearBufferData(lua_State* L) {
        dumpStack(L);
        lua_getglobal(L, "__sceneScript");
        void* ptr = lua_touserdata(L, -1);
        SceneScript::Impl* sceneScript = reinterpret_cast<SceneScript::Impl*>(ptr);
        sceneScript->GetUserBufferData().Clear();
        lua_pushboolean(L,true);
        return 1;
    }
    
    int getBufferDataNames(lua_State* L) {
        lua_getglobal(L, "__sceneScript");
        const void* ptr = lua_touserdata(L, -1);
        if (!ptr) {
            fprintf(stderr,"Invalid sceneScript instance\n");
            lua_pushnil(L);
            return 1;
        }

        LuaTable t;
        SceneScript::Impl* sceneScript = reinterpret_cast<SceneScript::Impl*>(const_cast<void*>(ptr));
        std::vector<std::string> ids;
        sceneScript->GetUserBufferData().GetBufferDataNames(ids);
        for (int i = 0, size = static_cast<int>(ids.size()); i < size; ++i) {
            t.push(ids[i]);
        }
        t.pushLuaTableValue(L);
        return 1;
    }
    
} // namespace



SceneScript::SceneScript() { m_imp = new Impl(); }
SceneScript::~SceneScript(){ delete m_imp; }
bool SceneScript::ExecuteFile(const char* scenefile, const std::vector<std::string>& sceneargs) {
    return m_imp->ExecuteFile(scenefile, sceneargs);
}
bool SceneScript::Execute(const char* luascript, const std::vector<std::string>& sceneargs) {
    return m_imp->Execute(luascript, sceneargs);
}
void SceneScript::Begin(const std::vector<std::string>& sceneargs) {
    m_imp->Begin(sceneargs);
}
void SceneScript::End() {
    m_imp->End();
}
bool SceneScript::Command(const char* sceneCommand) {
    return m_imp->Command(sceneCommand);
}

bool SceneScript::CreateMemoryData(const char* dataId)
{
    return m_imp->CreateMemoryData(dataId);
}
bool SceneScript::IsMemoryData(const char* dataId)
{
    return m_imp->IsMemoryData(dataId);
}
bool SceneScript::DeleteMemoryData(const char* dataId)
{
    return m_imp->DeleteMemoryData(dataId);
}
bool SceneScript::DeleteAllMemoryData()
{
    return m_imp->DeleteAllMemoryData();
}
bool SceneScript::SetMemoryData(const char* dataId, const void* memptr, const size_t memsize)
{
    return m_imp->SetMemoryData(dataId, memptr, memsize);
}
size_t SceneScript::GetMemoryDataSize(const char* dataId) const
{
    return m_imp->GetMemoryDataSize(dataId);
}
const void* SceneScript::GetMemoryDataPointer(const char* dataId) const
{
    return m_imp->GetMemoryDataPointer(dataId);
}
int SceneScript::GetMemoryDataNum() const
{
    return m_imp->GetMemoryDataNum();
}
const char* SceneScript::GetMemoryDataId(int i) const
{
    return m_imp->GetMemoryDataId(i);
}

UserBufferData* SceneScript::GetUserBufferData()
{
    return &m_imp->GetUserBufferData();
}

//----------------

bool SceneScript::Impl::ExecuteFile(const char* scenefile, const std::vector<std::string>& sceneargs)
{
    
    printf("Execute Scene file:%s\n", scenefile);
    
    FILE* fp = fopen(scenefile, "rb");
    if (!fp)
        return false;

    fseek(fp, 0, SEEK_END);
    const int scriptsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
#ifdef HIVE_STATIC
    std::string requireFunction = "\
        function LoadModule(name) \n \
            local m = loadstring('return type(' .. name .. ')')()  \n \
            if (m == 'table') then \n \
                return loadstring('return ' .. name)() \n \
            end \n \
            return loadstring('return ' .. name .. '()')() \n \
        end\n";
#else     
    std::string requireFunction = "\
        function LoadModule(name) \n \
            local m = require(name) \n \
            if (type(m) == 'table') then return m \n \
            else return m() end \n \
        end \n";
#endif
    
    char* luascript = new char[scriptsize + 1];
    size_t n = fread(luascript, scriptsize, 1, fp);
    if (n != 1) {
        return false;
    }
    luascript[scriptsize] = 0; // END

    bool r = Execute( (requireFunction + luascript).c_str(), sceneargs);
    
    delete [] luascript;
    
    return r;
}

bool SceneScript::Impl::Execute(const char* luascript, const std::vector<std::string>& sceneargs)
{
    Begin(sceneargs);
    bool r = Command(luascript);
    End();
    
    return r;
}

void SceneScript::Impl::Begin(const std::vector<std::string>& sceneargs)
{
    RegisterHiveCoreProfilingPoints();

    for (size_t i = 0; i < sceneargs.size(); ++i) {
        if (sceneargs[i] == "--opengl") {
            RenderCore::GetInstance(RENDER_OPENGL); // Switch OpenGL mode
        }
    }
    m_L = createLua();
    registerFuncs(m_L, this);
    
    // Reference path bin dir
    std::string includeBinDir = std::string("package.cpath=\"") + getBinaryDir() + std::string("?.\" .. dllExtension() .. \";\"..package.cpath");
    //printf("HH=%s",includeBinDir.c_str());
    doLua(m_L, includeBinDir.c_str());
    
    if (!sceneargs.empty()) {
        registerArg(m_L, sceneargs);
    }
}

bool SceneScript::Impl::Command(const char* sceneCommand)
{
    return doLua(m_L, sceneCommand);
}

void SceneScript::Impl::End()
{
    if (m_L) {
        closeLua(m_L);
    }
    m_L = 0;

    RenderCore::Finalize();
}


bool SceneScript::Impl::CreateMemoryData(const char* dataId)
{
    UserMemoryDataArray::const_iterator it = getUserMemoryData(dataId);
    if (it != m_memoryData.end()) {
        return false;
    }
    m_memoryData.push_back(UserMemoryData(std::string(dataId), 0, 0));
    return true;
}
bool SceneScript::Impl::IsMemoryData(const char* dataId)
{
    UserMemoryDataArray::const_iterator it = getUserMemoryData(dataId);
    if (it != m_memoryData.end()) {
        return false;
    }
    return true;
}
bool SceneScript::Impl::DeleteMemoryData(const char* dataId)
{
    UserMemoryDataArray::iterator it = getUserMemoryData(dataId);
    if (it != m_memoryData.end()) {
        m_memoryData.erase(it);
        return true;
    }
    return false;
}
bool SceneScript::Impl::DeleteAllMemoryData()
{
    m_memoryData.clear();
    return true;
}
bool SceneScript::Impl::SetMemoryData(const char* dataId, const void* memptr, const size_t memsize)
{
    UserMemoryDataArray::iterator it = getUserMemoryData(dataId);
    if (it == m_memoryData.end()) {
        return false;
    }
    
    it->ptr  = memptr;
    it->size = memsize;
    
    return true;
}
size_t SceneScript::Impl::GetMemoryDataSize(const char* dataId)
{
    UserMemoryDataArray::iterator it = getUserMemoryData(dataId);
    if (it == m_memoryData.end()) {
        return 0;
    }
    return it->size;
}
const void* SceneScript::Impl::GetMemoryDataPointer(const char* dataId)
{
    UserMemoryDataArray::iterator it = getUserMemoryData(dataId);
    if (it == m_memoryData.end()) {
        return 0;
    }
    return it->ptr;
}
int SceneScript::Impl::GetMemoryDataNum()
{
    return m_memoryData.size();
}
const char* SceneScript::Impl::GetMemoryDataId(int i)
{
    if (i >= m_memoryData.size()) {
        return "";
    }
    return m_memoryData[i].name.c_str();
}

SceneScript::Impl::UserMemoryDataArray::iterator SceneScript::Impl::getUserMemoryData(const char* dataId)
{
    const size_t n = m_memoryData.size();
    const std::string dataname(dataId);
    for (size_t i = 0; i < n; ++i) {
        if (m_memoryData[i].name == dataname) {
            return m_memoryData.begin() + i;
        }
    }
    return m_memoryData.end();
}

void SceneScript::Impl::PushMemoryData(const char* dataId)
{
    UserMemoryDataArray::const_iterator it = getUserMemoryData(dataId);
    LuaTable t;
    t.map("pointer", const_cast<void*>(it->ptr));
    t.map("size", it->size);
    t.pushLuaTableValue(m_L);
}


