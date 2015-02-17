#include <stdio.h>
#include <string.h>

#include <string>

#include "HdmLoader.h"
#include "SimpleVOL.h"

#ifndef HIVE_WITH_HDMLIB
#error "HIVE_WITH_HDMLIB must be defined when you compile HDMLoader module"
#endif

#include "BlockFactory.h"
#include "Block.h"
#include "BlockManager.h"
#include "Scalar3D.h"

//#include "LeafBlockLoader.h"
#include "BCMFileLoader.h"
#include "BCMFileSaver.h"

// @todo { implement }

HDMLoader::HDMLoader()
{
    Clear();
}
HDMLoader::~HDMLoader()
{
    Clear();
}

void HDMLoader::Clear()
{
    m_volume.Clear();
}

bool HDMLoader::Load(const char* cellidFilename, const char* dataFilename)
{
    Clear();

	// cellid.bcm
	BCMFileIO::BCMFileLoader loader(cellidFilename, /* bscatrer */NULL);

	BlockManager& blockManager = BlockManager::getInstance();
    blockManager.printBlockLayoutInfo();

	// data.bcm
	if( !loader.LoadAdditionalIndex(dataFilename) ){
		printf("err : Load File Error %s)\n", dataFilename);
	    return false;
	}



    return false;
}

int HDMLoader::Width()    {
    return m_volume.Width();
}

int HDMLoader::Height()   {
    return m_volume.Height();
}

int HDMLoader::Depth()    {
    return m_volume.Depth();
}

int HDMLoader::Component() {
    return m_volume.Component();
}

FloatBuffer* HDMLoader::Buffer() {
    return m_volume.Buffer();
}

BufferVolumeData *HDMLoader::VolumeData()
{
    return &m_volume;
}

