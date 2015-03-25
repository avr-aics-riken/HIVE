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
#include "Buffer.h"

class GenTexture::Impl
{
private:
	BufferImageData m_image;

public:
	Impl() {
		m_image.Clear();
	}

	~Impl() {
		m_image.Clear();
	}

	BufferImageData* ImageData()
	{
		return &m_image;
	}

	bool Create2D_RGBA8(unsigned char *buf, int width, int height )
	{
		printf("Width = %d\n", width);
		printf("Height = %d\n", height);
		m_image.Create(BufferImageData::RGBA8, width, height);
		memcpy(m_image.ImageBuffer()->GetBuffer(), buf, sizeof(unsigned char) * 4 * width * height);
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

BufferImageData* GenTexture::ImageData()
{
	return m_imp->ImageData();
}

const GenTexture::Buffer GenTexture::ImageBuffer() const
{
	return m_imp->ImageBuffer();
}

int GenTexture::ImageBufferSize() const
{
	return m_imp->ImageBufferSize();
}
