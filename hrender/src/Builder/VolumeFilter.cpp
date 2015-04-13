/**
 * @file VolumeFilter.cpp  
 * Apply filter for VolumeData, then generate another VolumeData
 */
#include "VolumeFilter.h"
#include <cstring>
#include <cmath>

namespace {

inline size_t IDX(size_t comps, size_t x, size_t y, size_t z, size_t c, size_t sx, size_t sy, size_t sz) {
    size_t dx = (std::max)((size_t)0, (std::min)(sx-1, x));
    size_t dy = (std::max)((size_t)0, (std::min)(sy-1, y));
    size_t dz = (std::max)((size_t)0, (std::min)(sz-1, z));
    size_t idx = comps * (dz * sx * sy + dy * sx + dx) + c;
    return idx;
}

template<typename T>
inline float D(const T* data, size_t comps, size_t x, size_t y, size_t z, size_t c, size_t sx, size_t sy, size_t sz) {
    size_t dx = (std::max)((size_t)0, (std::min)(sx-1, x));
    size_t dy = (std::max)((size_t)0, (std::min)(sy-1, y));
    size_t dz = (std::max)((size_t)0, (std::min)(sz-1, z));
    size_t idx = comps * (dz * sx * sy + dy * sx + dx) + c;
    return data[idx];
}


template<typename T>
inline T laplacian7(const T* data, size_t x, size_t y, size_t z, size_t sx, size_t sy, size_t sz)
{
    T xyz = D(data, 1, x, y, z, 1, sx, sy, sz);

    T x1 = D(data, 1, x+1, y, z, 1, sx, sy, sz);
    T x0 = D(data, 1, x-1, y, z, 1, sx, sy, sz);

    T y1 = D(data, 1, x, y+1, z, 1, sx, sy, sz);
    T y0 = D(data, 1, x, y-1, z, 1, sx, sy, sz);

    T z1 = D(data, 1, x, y, z+1, 1, sx, sy, sz);
    T z0 = D(data, 1, x, y, z-1, 1, sx, sy, sz);

    return x1 + x0 + y1 + y0 + z1 + z0 - 6.0 * xyz;
}

} // namespace

VolumeFilter::VolumeFilter() {
	Clear();
}

VolumeFilter::~VolumeFilter() {
	Clear();
}

void VolumeFilter::Clear() {
	m_filteredVolume = 0;
}

int VolumeFilter::Laplacian(BufferVolumeData *volume) {
    if (!volume) {
        return 0;
    }

	if (volume->Component() != 1) {
		fprintf(stderr, "[VolumeFilter] Scalar volume expected for Laplacian filter.\n");
		return 0;
	}

	Clear();

    const size_t width  = volume->Width();
    const size_t height = volume->Height();
    const size_t depth  = volume->Depth();
	const size_t components  = volume->Component();

	// Assume float data
	const float* src = volume->Buffer()->GetBuffer();

	m_filteredVolume = new BufferVolumeData();
	m_filteredVolume->Create(width, height, depth, 1); // scalar volume
	float* dst = m_filteredVolume->Buffer()->GetBuffer();

	#ifdef _OPENMP
	#pragma omp parallel for
	#endif
	for (size_t z = 0; z < depth; z++) {
		for (size_t y = 0; y < height; y++) {
			for (size_t x = 0; x < width; x++) {
				double lap = laplacian7(src, x, y, z, width, height, depth);
				dst[IDX(1, x, y, z, 0, width, height, depth)] = lap;
			}
		}
	}
	
    return 1; // OK
}

int VolumeFilter::Curl(BufferVolumeData *volume) {
    if (!volume) {
        return 0;
    }

	assert(0); // @todo

    return 0; // NG
}

int VolumeFilter::Gradient(BufferVolumeData *volume) {
    if (!volume) {
        return 0;
    }

	assert(0); // @todo

    return 0; // NG
}

int VolumeFilter::Norm(BufferVolumeData *volume) {
    if (!volume) {
        return 0;
    }

	Clear();

    const size_t width  = volume->Width();
    const size_t height = volume->Height();
    const size_t depth  = volume->Depth();
	const size_t components  = volume->Component();

	// Assume float data
	const float* src = volume->Buffer()->GetBuffer();

	m_filteredVolume = new BufferVolumeData();
	m_filteredVolume->Create(width, height, depth, 1); // scalar volume
	float* dst = m_filteredVolume->Buffer()->GetBuffer();

	#ifdef _OPENMP
	#pragma omp parallel for
	#endif
	for (size_t z = 0; z < depth; z++) {
		for (size_t y = 0; y < height; y++) {
			for (size_t x = 0; x < width; x++) {
				double sum = 0.0;
				for (size_t c = 0; c < components; c++) {
					double val = D(src, components, x, y, z, c, width, height, depth);
					sum += val * val;
				}
				double norm = sqrt(sum);
				
				dst[IDX(1, x, y, z, 0, width, height, depth)] = norm;
			}
		}
	}
	
    return 1; // OK
}

BufferVolumeData* VolumeFilter::VolumeData()
{
    return m_filteredVolume;
}

