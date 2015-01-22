//
//  hrender
//

#ifdef ENABLE_MPI
#include <mpi.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define chdir _chdir
#else
#include <unistd.h>
#endif

const int defWidth = 512;
const int defHeight = 512;

void renderScene(const char* scenefile)
{
    printf("RENDER!! > %s\n", scenefile);
    
    // Create RenderCore
    //RenderCore* render = new RenderCore(width,height);
    //render->SetMode(RENDER_LSGL);
    //render->SetFSAA(fsaa);
    
    // Load scenefile
    //std::string basepath = getBasePath(scenefile);
    //chdir(basepath.c_str());
    //std::string scenefilestr = getFilename(scenefile);
    //LoaderCore::LoadScene(scenefilestr.c_str(), rendering, render); // render from scene file
}

int main(int argc, char* argv[])
{
    int rank = 0;
#ifdef ENABLE_MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("[MPI] rank = %d\n", rank);
#endif
    
    char* scenefile = 0;
    
    // Parse args
    for (int i = 1; i < argc; ++i) {
        const char* arg = &argv[i][0];
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

