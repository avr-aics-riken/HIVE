/**
 * @file solversample.cpp
 */
//
//  HiveCore Solver sample
//

#ifdef HIVE_ENABLE_MPI
#include <mpi.h>
#endif

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>

#include "SceneScript/SceneScript.h"

#include "Core/Path.h"
#include "Core/sleep.h"
#include "Core/Perf.h"


/**
 * sceneレンダリングコア関数
 */
void renderScene(SceneScript& script, const char* scenefile, const std::vector<std::string>& sceneargs)
{
    printf("RENDER!! > %s\n", scenefile);
    
    std::string scenepath(scenefile);
    std::string scenefullfile = convertFullpath(scenepath);
    std::string cwddir = getCurrentDir();
    changeFileDir(scenefullfile); // set current to scenefile directory

    if (!script.ExecuteFile(scenefullfile.c_str(), sceneargs)) {
        fprintf(stderr, "[Error] scene file!! > %s\n", scenefullfile.c_str());
    }
}

int main(int argc, char* argv[])
{
#ifdef HIVE_ENABLE_MPI
    int rank = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //printf("[MPI] rank = %d\n", rank);
#endif

    // Initialize performance monitor.
    PMon::Init();
    
    SceneScript script;
    script.CreateMemoryData("testdata");
    
    const int testsize = 1024*1024*4;
    unsigned char* testdata = new unsigned char[testsize];
    for (int y = 0; y < 1024; ++y) {
        for (int x = 0; x < 1024; ++x) {
            testdata[4*(y * 1024 + x)  ] = static_cast<unsigned char>(x/1024.0*255);
            testdata[4*(y * 1024 + x)+1] = static_cast<unsigned char>(y);
            testdata[4*(y * 1024 + x)+2] = static_cast<unsigned char>(0);
            testdata[4*(y * 1024 + x)+3] = static_cast<unsigned char>(255);
        }
    }
    script.SetMemoryData("testdata", testdata, testsize);
    
    printf("C++: setMemoryData(%s, %p, %d)\n", "testdata", testdata, testsize);
    
    std::vector<std::string> sceneargs;
    renderScene(script, "./solversample.lua", sceneargs);
    
    delete [] testdata;
    
    // Report performance.
    PMon::Report();
    
#ifdef HIVE_ENABLE_MPI
    printf("[MPI] finalize at rank: %d\n", rank);
    MPI_Finalize();
#endif
    return 0;
}

