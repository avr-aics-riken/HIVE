/**
 * @file main.cpp
 * hrender main エントリポイント
 */
//
//  hrender
//

#ifdef HIVE_ENABLE_MPI
#include <mpi.h>
#endif

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

#include "../SceneScript/SceneScript.h"

#include "../Core/Path.h"
#include "../Core/Perf.h"

#ifdef _WIN32
	#define strncasecmp(x,y,z) _strnicmp(x,y,z)

	#ifdef _DEBUG
		#define _CRTDBG_MAP_ALLOC
		#include <crtdbg.h>
	#endif
#endif

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
    if (!script.Execute(scenefullfile.c_str(), sceneargs)) {
        fprintf(stderr, "[Error] scene file!! > %s\n", scenefullfile.c_str());
    }
}

/**
 * hrender main エントリポイント
 * @param argc コマンドライン引数数
 * @param argv コマンドライン引数文字列
 * @return 常に成功を返却する
 */

int main(int argc, char* argv[])
{
#ifdef _WIN32
	#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
		_CrtSetBreakAlloc(1037);
	#endif
#endif

    int rank = 0;
#ifdef HIVE_ENABLE_MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //printf("[MPI] rank = %d\n", rank);
#endif

    char* scenefile = 0;
    std::vector<std::string> sceneargs;
    
    // Parse args
    for (int i = 1; i < argc; ++i) {
        //const char* arg = &argv[i][0];
        size_t na = strlen(argv[i]);
        if (i == 1
        && na > 4
        && (strncasecmp(&argv[i][na-4],".scn",4) == 0 || strncasecmp(&argv[i][na-4],".lua",4) == 0))
        {
            scenefile = argv[i];
        }
        if (i > 1 && na < 2048) {
            sceneargs.push_back(argv[i]);
        }
    }
    
    if (argc <= 1 || scenefile == 0) {
		if (rank == 0) {
			printf("\n  Usage: hrender scenefile.scn\n\n");
		}

#ifdef HIVE_ENABLE_MPI
        MPI_Finalize();
#endif
        return 0;
    }

#ifdef HIVE_WITH_PMLIB
    printf("Initialize PM\n");
    InitPM();
#endif
    
#ifdef HIVE_WITH_PMLIB
    GetPM().start(HIVE_PERF_LABEL_CORE_MAIN);
#endif
    renderScene(scenefile, sceneargs);
#ifdef HIVE_WITH_PMLIB
    GetPM().stop(HIVE_PERF_LABEL_CORE_MAIN);
#endif
    
    printf("Exit hrender.\n");

#ifdef HIVE_WITH_PMLIB
    printf("Print PM\n");
    FinalizePM();
#endif
    
#ifdef HIVE_ENABLE_MPI
    printf("[MPI] finalize at rank: %d\n", rank);
    MPI_Finalize();
#endif
    return 0;
}

