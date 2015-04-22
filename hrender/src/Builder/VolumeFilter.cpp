/**
 * @file VolumeFilter.cpp  
 * Apply filter for VolumeData, then generate another VolumeData
 */
#include "VolumeFilter.h"
#include "CompileUtil.h"    // For user-defined volume filter.

#include <cstring>
#include <cmath>
#include <sstream>

namespace {

typedef struct {
    int width;
    int height;
    int depth;
    int components;
    int numSrcs;
    const float* src0;
    const float* src1;
    const float* src2;
    const float* src3; // Up to 4 source volumes
    float* dst;
} ArgInfo;

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

int VolumeFilter::Eval(BufferVolumeData *volume0, BufferVolumeData *volume1, BufferVolumeData *volume2, BufferVolumeData *volume3, int targetNumberOfComponents, const char* filterCode) {
    if (!volume0) {
        return 0;
    }

	Clear();

    hiveutil::CompileUtil cutil;

    cutil.SetCompiler("gcc", "-O2"); // @fixme

    std::stringstream ss;

    // Wrap filter code with header and footer
    ss << "#include <stdio.h>" << std::endl;
    ss << "typedef struct {" << std::endl;
    ss << "    int width;"  << std::endl;
    ss << "    int height;" << std::endl;
    ss << "    int depth;" << std::endl;
    ss << "    int components;" << std::endl;
    ss << "    int numSrcs;" << std::endl;
    ss << "    const float* src0;" << std::endl; 
    ss << "    const float* src1;" << std::endl;
    ss << "    const float* src2;" << std::endl;
    ss << "    const float* src3;" << std::endl;
    ss << "    float* dst;" << std::endl;
    ss << "} ArgInfo;" << std::endl;

    ss << "inline size_t IDX(size_t comps, size_t x, size_t y, size_t z, size_t c, size_t sx, size_t sy, size_t sz) {" << std::endl;
    ss << "  size_t dx = x; dx = (dx > (sx-1)) ? (sx-1) : dx;" << std::endl;
    ss << "  size_t dy = y; dy = (dy > (sy-1)) ? (sy-1) : dy;" << std::endl;
    ss << "  size_t dz = z; dz = (dz > (sz-1)) ? (sz-1) : dz;" << std::endl;
    ss << "  size_t idx = comps * (dz * sx * sy + dy * sx + dx) + c;" << std::endl;
    ss << "  return idx;" << std::endl;
    ss << "}" << std::endl;

    ss << "void *filter(void *args) {" << std::endl;
    ss << "  if (!args) return NULL;" << std::endl;
    ss << "  ArgInfo* info = (ArgInfo*)args;" << std::endl;
    ss << "  int width = info->width;" << std::endl;
    ss << "  int height = info->height;" << std::endl;
    ss << "  int depth = info->depth;" << std::endl;
    ss << "  int components = info->components; // of target volume" << std::endl;
    ss << "  int numSrcs = info->numSrcs;" << std::endl;
    ss << "  const float* src0 = info->src0;" << std::endl;
    ss << "  const float* src1 = info->src1;" << std::endl;
    ss << "  const float* src2 = info->src2;" << std::endl;
    ss << "  const float* src3 = info->src3;" << std::endl;
    ss << "  float* dst = info->dst;" << std::endl;

	ss << "  for (size_t z = 0; z < depth; z++) {" << std::endl;
	ss << "  	for (size_t y = 0; y < height; y++) {" << std::endl;
	ss << "  		for (size_t x = 0; x < width; x++) {" << std::endl;

    ss << std::string(filterCode, strlen(filterCode)) << std::endl; // strip '\0'

    ss << "  } } }" << std::endl;
    ss << "  return NULL; // @todo" << std::endl;
    ss << "}" << std::endl;

    printf(ss.str().c_str());

    cutil.SetSource(ss.str());
    bool ret = cutil.Compile("filter");
    if (!ret) {
        return 0;
    }

    // Assume all src volumes has same extent and same # of components.
    ArgInfo info;
    info.components = targetNumberOfComponents;
    info.width = volume0->Width();
    info.height = volume0->Height();
    info.depth = volume0->Depth();
    info.src0 = volume0->Buffer()->GetBuffer();
    info.src1 = NULL;
    if (volume1) {
        info.src1 = volume1->Buffer()->GetBuffer();
    }
    if (volume2) {
        info.src2 = volume2->Buffer()->GetBuffer();
    }
    if (volume3) {
        info.src3 = volume3->Buffer()->GetBuffer();
    }

	m_filteredVolume = new BufferVolumeData();
	m_filteredVolume->Create(info.width, info.height, info.depth, targetNumberOfComponents);
	float* dst = m_filteredVolume->Buffer()->GetBuffer();
    info.dst = dst;

    cutil.Eval(&info);

    return 1; // OK
}

BufferVolumeData* VolumeFilter::VolumeData()
{
    return m_filteredVolume;
}

