/**
 * @file VolumeFilter.h
 * Apply filter for VolumeData, then generate another VolumeData
 */
#ifndef _VOLUMEFILTER_H_
#define _VOLUMEFILTER_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>
#include <string>
#include "BufferVolumeData.h"
#include "BufferTetraData.h"
#include "BufferLineData.h"
#include "BufferVectorData.h"
#include "CompileUtil.h"

class VolumeFilter : public RefCount
{
private:
	BufferVolumeData* m_filteredVolume;
	hiveutil::CompileUtil m_cutil;
	std::string m_compiler;
	std::string m_compilerOpts;

public:
    VolumeFilter();
	~VolumeFilter();

	void Clear();

	// Apply laplacian filter to the input volume
	// @return 1 success, 0 failed
    int Laplacian(BufferVolumeData *volume);

	// Apply curl filter to the input volume
	// @return 1 success, 0 failed
    int Curl(BufferVolumeData *volume);

	// Apply gradient filter to the input volume
	// @return 1 success, 0 failed
    int Gradient(BufferVolumeData *volume);

	// Compute (vector) norm of volume cell.
	// @return 1 success, 0 failed
    int Norm(BufferVolumeData *volume);

	// Set compiler and compile options
	// @return 1 success, 0 failed
    int SetCompileOption(const char* compiler, const char* compileOpts);

	// Apply user defined filter code for volume cell.
  // You can specify up to 4 source volumes. volume0 must be not null.
	// @return 1 success, 0 failed
    int Expr(BufferVolumeData *volume0, BufferVolumeData *volume1, BufferVolumeData *volume2, BufferVolumeData *volume3, int targetNumberOfComponents, const char* source);

	// Retrieve filtered volume data.
	// Must be called after the one of volume filtering operation(e.g. Laplacian())
	BufferVolumeData* VolumeData();

};

#endif //_VOLUMEFILTER_H_

