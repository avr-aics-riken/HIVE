#ifndef INCLUDE_SIMPLEPNG
#define INCLUDE_SIMPLEPNG

#include "lodepng.h"

#include <stdlib.h>
#include <string.h>

//=====================================================================================================================

inline int SimplePNGSaverRGBA(void** pngbuffer, int w, int h, const unsigned char* rgba)
{
	if (!pngbuffer)
		return 0;

  // flip Y
  unsigned char* pImage = new unsigned char[w*h*4];
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      pImage[4*(x + y * w)  ] = rgba[4*(x + (h - 1 - y) * w)  ];
      pImage[4*(x + y * w)+1] = rgba[4*(x + (h - 1 - y) * w)+1];
      pImage[4*(x + y * w)+2] = rgba[4*(x + (h - 1 - y) * w)+2];
      pImage[4*(x + y * w)+3] = rgba[4*(x + (h - 1 - y) * w)+3];
    }
  }

  size_t out_size = 0;
  std::vector<unsigned char> out;
  unsigned int error = lodepng::encode(out, pImage, w, h);

  delete [] pImage;
	
  if (error) {
    fprintf(stderr, "PNG encode error: %s\n", lodepng_error_text(error));
    return 0;
  }

  unsigned char* buf = new unsigned char[out.size()]; // callee must free this buffer.
  memcpy(buf, &out.at(0), out.size());
  (*pngbuffer) = reinterpret_cast<void*>(buf);
  return out.size();
}

#endif // INCLUDE_SIMPLEPNG
