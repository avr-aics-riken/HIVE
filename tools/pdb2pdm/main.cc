#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cmath>

#include <string>

#include "tiny_pdb.h"

#ifdef HIVE_ENABLE_MPI
#include <mpi.h>
#endif

int main(int argc, char **argv) {
#ifdef HIVE_ENABLE_MPI
  int rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif
  if (argc < 3) {
    fprintf(stderr, "need input.pdb output.pdm\n");
    exit(-1);
  }

  tinypdb::TinyPDB pdb(argv[1]);
  if (pdb.Parse(/* isBondGenerated = */ true)) {
    fprintf(stderr, "PDB parsing failed: %s \n", argv[1]);
    return -1;
  }

#ifdef HIVE_ENABLE_MPI
  MPI_Finalize();
#endif

  return 0;
}
