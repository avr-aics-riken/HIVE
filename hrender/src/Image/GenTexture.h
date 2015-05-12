/**
 * @file GenTexture.h
 * 画像ローダ
 */
#ifndef _GENTEXTURE_H_
#define _GENTEXTURE_H_

#include "Ref.h"

class BufferImageData;
class BufferVolumeData;

class GenTexture : public RefCount
{
private:
	class Impl;
	Impl* m_imp;

public:
	typedef void* Buffer;

	GenTexture();
	~GenTexture();
	bool Create2D_RGBA8(unsigned char *buf, int width, int height );
	bool Create2D_F32(float *buf, int width, int height );
	bool Create2D_RGBA32(float *buf, int width, int height );
	bool Create3D_F32(float *buf, int width, int height, int depth);
	BufferImageData *ImageData();
	BufferVolumeData *VolumeData();
	const Buffer ImageBuffer() const;

	int ImageBufferSize() const;

};

#endif //_GENTEXTURE_H_
