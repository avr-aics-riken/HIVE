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

#include "../SceneScript/SceneScript.h"

#include "../Core/Path.h"

#ifdef _WIN32
	#define strncasecmp(x,y,z) _strnicmp(x,y,z)
#endif

/**
 * sceneレンダリングコア関数
 */
void renderScene(const char* scenefile)
{
    printf("RENDER!! > %s\n", scenefile);
    
    std::string scenepath(scenefile);
    std::string scenefullfile = convertFullpath(scenepath);
    changeFileDir(scenefullfile);
    
    SceneScript script;
    if (!script.Execute(scenefullfile.c_str())) {
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
    int rank = 0;
#ifdef HIVE_ENABLE_MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //printf("[MPI] rank = %d\n", rank);
#endif
    
    char* scenefile = 0;
    
    // Parse args
    for (int i = 1; i < argc; ++i) {
        //const char* arg = &argv[i][0];
        size_t na = strlen(argv[i]);
        if (na > 4
        && (strncasecmp(&argv[i][na-4],".scn",4) == 0 || strncasecmp(&argv[i][na-4],".lua",4) == 0))
        {
            scenefile = argv[i];
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
    
    renderScene(scenefile);
    
    printf("Exit hrender.\n");
    
#ifdef HIVE_ENABLE_MPI
    printf("[MPI] finalize at rank: %d\n", rank);
    MPI_Finalize();
#endif
    return 0;
}

