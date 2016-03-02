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

#define STRINGIFY(A) #A

int main(int argc, char* argv[])
{
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

