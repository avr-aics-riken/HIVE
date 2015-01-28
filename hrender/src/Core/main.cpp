//
//  hrender
//

#ifdef ENABLE_MPI
#include <mpi.h>
#endif

#include <stdio.h>
#include <string.h>
#include <string>

const int defWidth = 512;
const int defHeight = 512;

#include "../SceneScript/SceneScript.h"

#include "../Core/Path.h"

void renderScene(const char* scenefile)
{
    printf("RENDER!! > %s\n", scenefile);
    
    std::string scenepath(scenefile);
    std::string scenefullfile = convertFullpath(scenepath);
    changeFileDir(scenefullfile);
    
    SceneScript script;
    if (!script.Execute(scenefullfile.c_str())) {
        printf("[Error] scene file!! > %s\n", scenefullfile.c_str());
    }
}

int main(int argc, char* argv[])
{
#ifdef ENABLE_MPI
    int rank = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("[MPI] rank = %d\n", rank);
#endif
    
    char* scenefile = 0;
    
    // Parse args
    for (int i = 1; i < argc; ++i) {
        //const char* arg = &argv[i][0];
        size_t na = strlen(argv[i]);
        if (na > 4 && strncasecmp(&argv[i][na-4],".scn",4)==0)
        {
            scenefile = argv[i];
        }
    }
    
    if (argc <= 1 || scenefile == 0) {
        printf("\n  Usage: hrender scenefile.scn\n\n");
        return 0;
    }
    
    renderScene(scenefile);
    
    printf("Exit hrender.\n");
    
#ifdef ENABLE_MPI
    MPI_Finalize();
#endif
    return 0;
}

