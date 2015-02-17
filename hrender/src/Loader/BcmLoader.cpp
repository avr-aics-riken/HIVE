#include <stdio.h>
#include <string.h>

#include <string>

#include <mpi.h>

#include "BcmLoader.h"
#include "SimpleVOL.h"

#ifndef HIVE_WITH_BCMTOOLS
#error "HIVE_WITH_BCMTOOLS must be defined when you compile BCMLoader module"
#endif

#include "BCMTools.h"
#include "BCMOctree.h"
#include "PolygonBBoxDivider.h"

//#include "Config.h"


BCMLoader::BCMLoader()
{
    Clear();
}
BCMLoader::~BCMLoader()
{
    Clear();
}

void BCMLoader::Clear()
{
    m_volume.Clear();
}

bool BCMLoader::Load(const char* filename)
{
    Clear();

	// Assume MPI_Init() has been called in hrender::main()

	int rank = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	BCMOctree* tree;
	if (rank == 0) {
		int N = 1; // @fixme
		RootGrid* rootGrid = new RootGrid(N);
		Divider* divider = NULL; // @fixme

		//Divider* divider = new PolygonBBoxDivider(conf.origin, conf.rootLength,
		//										  rootGrid, conf.minLevel, pl,
		//										  conf.polygonGroupList,
		//										  conf.boundingBoxList,
		//										  (double)conf.vc/conf.size);

		BCMOctree::Ordering ordering = BCMOctree::Z; // @fixme

		tree = new BCMOctree(rootGrid, divider, ordering);
	} else {
		tree = BCMOctree::ReceiveFromMaster();
	}
	// @todo {}

    return false;
}

int BCMLoader::Width()    {
    return m_volume.Width();
}

int BCMLoader::Height()   {
    return m_volume.Height();
}

int BCMLoader::Depth()    {
    return m_volume.Depth();
}

int BCMLoader::Component() {
    return m_volume.Component();
}

FloatBuffer* BCMLoader::Buffer() {
    return m_volume.Buffer();
}

BufferVolumeData *BCMLoader::VolumeData()
{
    return &m_volume;
}

