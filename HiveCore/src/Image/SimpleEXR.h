//
//  SimpleEXR.h
//  for OpenEXR format
//
//

#ifndef INCLUDE_SimpleEXR_h
#define INCLUDE_SimpleEXR_h

#include "tinyexr.h"

#ifdef _WIN32
	#include <io.h>
#else
	#include <unistd.h>
#endif

inline bool SimpleEXRLoaderRGBA(const char* exrfilename, int& w, int& h, float** rgba_float)
{
  if (!rgba_float) {
    return false;
  }

  const char *err = NULL;
  float* buf = NULL;
  int ret = LoadEXR(&buf, &w, &h, exrfilename, &err);
  if (ret != 0 || (w == 0) || (h == 0)) {
    if (err) {
      fprintf(stderr, "[EXR] %s\n", err);
    }
    return false;
  }

  // Flip Y to match OpenGL coord(LowerLeft)
  (*rgba_float) = new float[w*h*4];
  for (size_t y = 0; y < h; y++) {
    for (size_t x = 0; x < w; x++) {
      (*rgba_float)[4*(y*w+x)+0] = buf[4*((h-1-y)*w+x)+0];
      (*rgba_float)[4*(y*w+x)+1] = buf[4*((h-1-y)*w+x)+1];
      (*rgba_float)[4*(y*w+x)+2] = buf[4*((h-1-y)*w+x)+2];
      (*rgba_float)[4*(y*w+x)+3] = buf[4*((h-1-y)*w+x)+3];
    }
  }

  free(buf);

  return true;
}

/// Saves a EXR image with RGBA channel to `out_mem` and return its data size.
inline int SimpleEXRSaverRGBA(void** out_mem, int w, int h, const float* rgba_float)
{
#ifdef _WIN32
	return -1;
#else
  // @todo { Write EXR directry to memory. }

  if (!rgba_float) {
    return 0;
  }

  EXRImage exr;
  exr.width = w;
  exr.height = h;
  exr.num_channels = 7; // RGBA + alpha premultipled RGB

  // Must be A,B,G,R order
  const char *channel_names[] = {"A", "B", "BA", "G", "GA", "R", "RA"};
  exr.channel_names = channel_names;
  
  float **channel_images = new float*[exr.num_channels];
  int *pixel_types = new int[exr.num_channels];
  for (size_t i = 0; i < exr.num_channels; i++) {
    channel_images[i] = new float[w*h];
	pixel_types[i] = TINYEXR_PIXELTYPE_HALF; // @fixme
  }

  for (size_t y = 0; y < h; y++) {
    for (size_t x = 0; x < w; x++) {
      float alpha = rgba_float[4*((h-1-y)*w+x)+3]; 
      channel_images[5][y*w+x] = rgba_float[4*((h-1-y)*w+x)+0]; // R
      channel_images[3][y*w+x] = rgba_float[4*((h-1-y)*w+x)+1]; // G
      channel_images[1][y*w+x] = rgba_float[4*((h-1-y)*w+x)+2]; // B
      channel_images[6][y*w+x] = rgba_float[4*((h-1-y)*w+x)+0] * alpha; // RA
      channel_images[4][y*w+x] = rgba_float[4*((h-1-y)*w+x)+1] * alpha; // GA
      channel_images[2][y*w+x] = rgba_float[4*((h-1-y)*w+x)+2] * alpha; // BA
      channel_images[0][y*w+x] = alpha; // A
    }
  }
  exr.images = reinterpret_cast<unsigned char**>(channel_images);
  exr.pixel_types = pixel_types;
  
  const char* err = NULL;
  unsigned char* mem = NULL;
  size_t mem_size = SaveMultiChannelEXRToMemory(&exr, &mem, &err);
  if (mem_size <= 0) {
    if (err) {
      fprintf(stderr, "[EXRSave] %s\n", err);
    }
    return 0;
  }

  for (size_t i = 0; i < exr.num_channels; i++) {
    delete [] channel_images[i];
  }
  delete [] channel_images;
  delete [] pixel_types;

  // malloc() -> new
  (*out_mem) = reinterpret_cast<void*>(new unsigned char[mem_size]);
  memcpy((*out_mem), mem, mem_size);

  free(mem);

	return mem_size;
#endif
}

/// Saves a EXR image with Z channel to `out_mem` and return its data size.
inline int SimpleEXRSaverZ(void** out_mem, int w, int h, const float* z_float)
{
#ifdef _WIN32
	return -1;
#else
  // @todo { Write EXR directry to memory. }

  if (!z_float) {
    return 0;
  }

  EXRImage exr;
  exr.width = w;
  exr.height = h;
  exr.num_channels = 1; // depth

  const char *channel_names[] = {"Z"};
  exr.channel_names = channel_names;
  
  float **channel_images = new float*[exr.num_channels];
  int *pixel_types = new int[exr.num_channels];
  for (size_t i = 0; i < exr.num_channels; i++) {
    channel_images[i] = new float[w*h];
	pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT; // @fixme
  }

  for (size_t y = 0; y < h; y++) {
    for (size_t x = 0; x < w; x++) {
      channel_images[0][y*w+x] = z_float[(h-1-y)*w+x];
    }
  }
  exr.images = reinterpret_cast<unsigned char**>(channel_images);
  exr.pixel_types = pixel_types;
  
  const char* err = NULL;
  unsigned char* mem = NULL;
  size_t mem_size = SaveMultiChannelEXRToMemory(&exr, &mem, &err);

  for (size_t i = 0; i < exr.num_channels; i++) {
    delete [] channel_images[i];
  }
  delete [] channel_images;
  delete [] pixel_types;

  // malloc() -> new
  (*out_mem) = reinterpret_cast<void*>(new unsigned char[mem_size]);
  memcpy((*out_mem), mem, mem_size);

  free(mem);

	return mem_size;
#endif
}

#endif // INCLUDE_SimpleEXR_h

