/**
 * @file VolumeFilter.h
 * Apply filter for VolumeData, then generate another VolumeData
 */
#ifndef _VOLUMEFILTER_H_
#define _VOLUMEFILTER_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>
#include "BufferVolumeData.h"
#include "BufferTetraData.h"
#include "BufferLineData.h"
#include "BufferVectorData.h"

class VolumeFilter : public RefCount
{
private:
	BufferVolumeData* m_filteredVolume;

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

	// Apply user defined filter code for volume cell.
  // You can specify up to 4 source volumes. volume0 must be not null.
	// @return 1 success, 0 failed
    int Eval(BufferVolumeData *volume0, BufferVolumeData *volume1, BufferVolumeData *volume2, BufferVolumeData *volume3, int targetNumberOfComponents, const char* source);

	// Retrieve filtered volume data.
	// Must be called after the one of volume filtering operation(e.g. Laplacian())
	BufferVolumeData* VolumeData();

};

#endif //_VOLUMEFILTER_H_

