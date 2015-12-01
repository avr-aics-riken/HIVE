/**
 * @file GenTexture.cpp
 * generate texture.
 */
#include "GenTexture.h"
#include <stdio.h>
#include <string>
#include <string.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "BufferImageData.h"
#include "BufferVolumeData.h"
#include "Buffer.h"

class GenTexture::Impl
{
private:
	BufferImageData m_image;
	BufferVolumeData m_volimage;

public:
	Impl() {
		m_image.Clear();
		m_volimage.Clear();
	}

	~Impl() {
		m_image.Clear();
		m_volimage.Clear();
	}

	BufferImageData* ImageData()
	{
		return &m_image;
	}

	BufferVolumeData* VolumeData()
	{
		return &m_volimage;
	}

	bool Create2D_RGBA8(unsigned char *buf, int width, int height )
	{
		m_image.Create(BufferImageData::RGBA8, width, height);
		memcpy(m_image.ImageBuffer()->GetBuffer(), buf, sizeof(unsigned char) * 4 * width * height);
		return true;
	}

	bool Create2D_F32(float *buf, int width, int height )
	{
		m_image.Create(BufferImageData::R32F, width, height);
		memcpy(m_image.FloatImageBuffer()->GetBuffer(), buf, sizeof(float) * width * height);
		return true;
	}

	bool Create2D_RGBA32(float *buf, int width, int height )
	{
		m_image.Create(BufferImageData::RGBA32F, width, height);
		memcpy(m_image.FloatImageBuffer()->GetBuffer(), buf, sizeof(float) * 4 * width * height);
		return true;
	}

	bool Create3D_F32(float *buf, int width, int height, int depth)
	{
		m_volimage.Create(width, height, depth, 1);
		memcpy(m_volimage.Buffer()->GetBuffer(), buf, sizeof(float) * width * height * depth);
		return true;
	}

	const Buffer ImageBuffer() const
	{
		unsigned char* buf = m_image.ImageBuffer()->GetBuffer();
		return reinterpret_cast<GenTexture::Buffer>(buf);
	}

	int ImageBufferSize() const
	{
		return m_image.ImageBuffer()->GetNum();
	}
};

GenTexture::GenTexture()
{
	m_imp = new GenTexture::Impl();
}

GenTexture::~GenTexture()
{
	delete m_imp;
}

bool GenTexture::Create2D_RGBA8(unsigned char *buf, int width, int height )
{
	return m_imp->Create2D_RGBA8(buf, width, height );
}

bool GenTexture::Create2D_F32(float *buf, int width, int height )
{
	return m_imp->Create2D_F32(buf, width, height);
}

bool GenTexture::Create2D_RGBA32(float *buf, int width, int height )
{
	return m_imp->Create2D_RGBA32(buf, width, height);
}

bool GenTexture::Create3D_F32(float *buf, int width, int height, int depth )
{
	return m_imp->Create3D_F32(buf, width, height, depth);
}

BufferImageData* GenTexture::ImageData()
{
	return m_imp->ImageData();
}

BufferVolumeData* GenTexture::VolumeData()
{
	return m_imp->VolumeData();
}


const GenTexture::Buffer GenTexture::ImageBuffer() const
{
	return m_imp->ImageBuffer();
}

int GenTexture::ImageBufferSize() const
{
	return m_imp->ImageBufferSize();
}
