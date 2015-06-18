/**
 * @file HdmLoader.h
 * HDMデータローダー
 */
#ifndef _HDMLOADER_H_
#define _HDMLOADER_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferSparseVolumeData.h"

// Forward decl.
namespace BCMFileIO {

class BCMFileLoader;

}

/**
 * HDMデータローダー
 */
class HDMLoader : public RefCount
{
  public:
	HDMLoader();
	~HDMLoader();
	void Clear();

	// Initialize HDM loading.
	// @note { Due to the API design of HDMLoader, Init() can be called only once in the program lifecycle.
	// (i.e. You cannot create two instance of HDMLoader.) }
	bool Init(const char *cellidFilename, const char *dataFilename);

	// Load field data of given name, type, compoents and timeStep. Valid after Init().
	BufferSparseVolumeData* LoadField(const char *fieldName, const char *fieldType, int components,
			  int timeStepIndex = 0, int virtualCells = 2);

  private:
	bool m_initialized;
	BCMFileIO::BCMFileLoader *m_loader;

	std::map<std::string, RefPtr<BufferSparseVolumeData> > m_fields;
};

#endif //_HDMLOADER_H_
