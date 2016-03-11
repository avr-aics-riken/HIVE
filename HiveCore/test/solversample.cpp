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
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <cassert>
#include <sstream>

#include "SceneScript/SceneScript.h"

#include "Core/Path.h"
#include "Core/sleep.h"
#include "Core/Perf.h"

// From $HIVE/include
#include "InSitu/HIVESparseVolume.h"

namespace {

unsigned char *LoadRawVolumeTexture(const char *filename, size_t size) {
  FILE *fp = fopen(filename, "rb");
  assert(fp);

  unsigned char *data = new unsigned char[size];
  size_t sz = fread(data, 1, size, fp);
  assert(sz == size);
  assert(data);

  fclose(fp);

  return data;
}

float *GenScalarVolumeBlock(int w, int h, int d)
{
    float* ptr = new float[w * h * d];

    for (int z = 0; z < d; z++) {
        int zz = z % 255;
        for (int y = 0; y < h; y++) {
            int yy = y % 255;
            for (int x = 0; x < w; x++) {
                int xx = x % 255;
                ptr[z * h * w + y * w + x] = xx + yy + zz;
            }
        }
    }

    return ptr;
}


void GenSparseVolumeScalarFloatTestData(/* out */HIVESparseVolume* sparseVolume, const float* voldata, int dim[3])
{

  int nlods = 4; // Up to 8 for demo;
  int loc[8][3];

  // Compute block origin.
  loc[0][0] = 0;
  loc[0][1] = 0;
  loc[0][2] = 0;
  for (int i = 1; i < nlods; i++) {

    loc[i][0] = 0; //loc[i-1][0] + dim[0] * pow(2.0, i-1);
    loc[i][1] = 0; //loc[i-1][1] + dim[1] * pow(2.0, i-1);
    loc[i][2] = loc[i-1][2] + dim[2] * (1 << (i-1));
  }

  sparseVolume->numBlocks = nlods;
  sparseVolume->blocks = new HIVEVolumeBlock[nlods];
  
  for (int i = 0; i < nlods; i++) {
  
    int extent[3] = {0,0,0};

    extent[0] = dim[0] * (1 << i);
    extent[1] = dim[1] * (1 << i);
    extent[2] = dim[2] * (1 << i);

    printf("loc    = %d, %d, %d\n", loc[i][0], loc[i][1], loc[i][2]);
    printf("extent = %d, %d, %d\n", extent[0], extent[1], extent[2]);
    printf("dim    = %d, %d, %d\n", dim[0], dim[1], dim[2]);

    sparseVolume->blocks[i].offset[0] = loc[i][0]; // offset. mandatory.
    sparseVolume->blocks[i].offset[1] = loc[i][1];
    sparseVolume->blocks[i].offset[2] = loc[i][2];

    sparseVolume->blocks[i].extent[0] = extent[0]; // extent. mandatory.
    sparseVolume->blocks[i].extent[1] = extent[1];
    sparseVolume->blocks[i].extent[2] = extent[2];

    sparseVolume->blocks[i].size[0] = dim[0]; // cell size. mandatory.
    sparseVolume->blocks[i].size[1] = dim[1];
    sparseVolume->blocks[i].size[2] = dim[2];

    sparseVolume->blocks[i].level = i; // LoD level. optional.
    sparseVolume->blocks[i].id = i; // block ID. optional.

    sparseVolume->blocks[i].data = reinterpret_cast<unsigned char*>(const_cast<float*>(voldata)); // share volume data for all volume blocks. 

  }

  // 1 = scalar
  sparseVolume->components = 1; // The number of components in volume blocks. Assume all volume blocks has same the number of components.

  sparseVolume->format = HIVE_VOLUME_FORMAT_FLOAT;

  // Optional
  // sparseVolume->globalDim[0] = ...
  // sparseVolume->globalDim[1] = ...
  // sparseVolume->globalDim[2] = ...

  sparseVolume->maxLevel = nlods - 1; // Optional. Maxium number of LoD level in sparse volume.
}

float* LoadRawVolumeTextureAndConvertToFloat(const char* filename, int dim[3])
{
    unsigned char* data = LoadRawVolumeTexture(filename, dim[0] * dim[1] * dim[2]);

    float *dst = new float[dim[0] * dim[1] * dim[2]];

    for (int i = 0; i < dim[0] * dim[1] * dim[2]; i++) {
        dst[i] = data[i] / 255.0f;
    }

    delete [] data;

    return dst;
}

} // namespace


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
    
    int volumeDim[3] = {32, 32, 32};

    printf("Usage: %s <file.raw dimX dimY dimZ>\n", argv[0]);

    float *volumeData = NULL;
    if (argc > 4) {
        volumeDim[0] = atoi(argv[2]);
        volumeDim[1] = atoi(argv[3]);
        volumeDim[2] = atoi(argv[4]);

        printf("Load Raw: %s, %d, %d, %d\n", argv[1], volumeDim[0], volumeDim[1], volumeDim[2]);
        volumeData = LoadRawVolumeTextureAndConvertToFloat(argv[1], volumeDim);
    } else {
        volumeData = GenScalarVolumeBlock(volumeDim[0], volumeDim[1], volumeDim[2]);
    }

    HIVESparseVolume* sparseVolume = new HIVESparseVolume;
    GenSparseVolumeScalarFloatTestData(sparseVolume, volumeData, volumeDim);

    SceneScript script;
    script.CreateMemoryData("SparseVolumeScalar");
    script.SetMemoryData("SparseVolumeScalar", sparseVolume, sizeof(HIVESparseVolume));
    
    printf("C++: setMemoryData(%s, %p, %d)\n", "SparseVolumeScalar", sparseVolume, sizeof(HIVESparseVolume));
    
    std::vector<std::string> sceneargs;
    renderScene(script, "./solversample.lua", sceneargs);
    
    // Delete volume data
    delete volumeData;
    delete sparseVolume;

    // Report performance.
    PMon::Report();
    
#ifdef HIVE_ENABLE_MPI
    printf("[MPI] finalize at rank: %d\n", rank);
    MPI_Finalize();
#endif

    return 0;
}

