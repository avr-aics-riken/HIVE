//
//  SimpleEXR.h
//  for OpenEXR format
//
//

#ifndef INCLUDE_SimpleEXR_h
#define INCLUDE_SimpleEXR_h

#include "tinyexr.h"

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
  // @todo
  if (!rgba_float) {
    return false;
  }

	return false;
}

/// Saves a EXR image with Z channel to `out_mem` and return its data size.
inline int SimpleEXRSaverZ(void** out_mem, int w, int h, const float* z_float)
{
  // @todo
  if (!z_float) {
    return false;
  }

	return false;
}

#endif // INCLUDE_SimpleEXR_h

