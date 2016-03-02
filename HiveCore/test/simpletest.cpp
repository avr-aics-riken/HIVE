/**
 * @file simpletest.cpp
 */
//
//  HiveCore Simple Test
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


/**
 * sceneレンダリングコア関数
 */
void renderScene(const char* scenefile, const std::vector<std::string>& sceneargs)
{
    printf("RENDER!! > %s\n", scenefile);
    
    std::string scenepath(scenefile);
    std::string scenefullfile = convertFullpath(scenepath);
    changeFileDir(scenefullfile);

    SceneScript script;
    if (!script.ExecuteFile(scenefullfile.c_str(), sceneargs)) {
        fprintf(stderr, "[Error] scene file!! > %s\n", scenefullfile.c_str());
    }
}

#define STRINGIFY(A) #A

int main(int argc, char* argv[])
{
#ifdef _WIN32
	#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
		_CrtSetBreakAlloc(1037);
	#endif
#endif

#ifdef HIVE_ENABLE_MPI
    int rank = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //printf("[MPI] rank = %d\n", rank);
#endif
    
    std::string scenescript = STRINGIFY(print('hello from scene script')\n
                                        local camera = Camera()\n
                                        camera:SetScreenSize(256,256);
                                        camera:SetFilename('simpletest.jpg')\n
                                        render({camera})
                                        );
    
    
    std::vector<std::string> sceneargs;
    SceneScript script;
    if (!script.Execute(scenescript.c_str(), sceneargs)) {
        fprintf(stderr, "[Error] scene script!!\n");
    }
    
#ifdef HIVE_ENABLE_MPI
    printf("[MPI] finalize at rank: %d\n", rank);
    MPI_Finalize();
#endif
    return 0;
}

