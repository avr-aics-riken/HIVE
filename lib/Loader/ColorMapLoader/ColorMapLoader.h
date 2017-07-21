/**
 * @file ColorMapLoader.h
 * ColorMapデータローダー
 */
#ifndef _COLORMAPLOADER_H_
#define _COLORMAPLOADER_H_

#include "Buffer.h"
#include "Ref.h"

/**
 * ColorMapデータローダー
 */
class ColorMapLoader : public RefCount
{
  public:
	ColorMapLoader();
	~ColorMapLoader();
	void Clear();
	RefPtr<FloatBuffer> m_rgba;
	float m_minValue;
	float m_maxValue;

  protected:
	bool Load(const char *filename);
	FloatBuffer *RGBAData() { return m_rgba; }
	int RGBASize() const;
	float RGBAValue(int pos) const;
	float MinValue() const { return m_minValue; }
	float MaxValue() const { return m_maxValue; }
};

#endif //_COLORMAPLOADER_H_
