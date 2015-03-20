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
  return true;
}

/// Saves a EXR image with RGBA channel to `out_mem` and return its data size.
inline int SimpleEXRSaverRGBA(void** out_mem, int w, int h, const float* rgba_float)
{
  if (!rgba_float) {
    return false;
  }

	return true;
}

/// Saves a EXR image with Z channel to `out_mem` and return its data size.
inline int SimpleEXRSaverZ(void** out_mem, int w, int h, const float* z_float)
{
  if (!z_float) {
    return false;
  }

	return true;
}

#endif // INCLUDE_SimpleEXR_h

