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
  int ret = LoadEXR(rgba_float, &w, &h, exrfilename, &err);
  if (ret != 0) {
    if (err) {
      fprintf(stderr, "[EXR] %s\n", err);
      return false;
    }
  }

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

