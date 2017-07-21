/**
 * @file GradientMapLoader.h
 * GradientMapデータローダー
 */
#ifndef _GRADIENTMAPLOADER_H_
#define _GRADIENTMAPLOADER_H_

#include "Buffer.h"
#include "Ref.h"

/**
 * GradientMapデータローダー
 */
class GradientMapLoader : public RefCount
{
  public:
	GradientMapLoader();
	~GradientMapLoader();
	void Clear();
	RefPtr<FloatBuffer> m_rgba;
	float m_minValue;
	float m_maxValue;

  protected:
	bool Load(const char *filename);
	FloatBuffer *RGBAData() { return m_rgba; }
	int RGBASize() const;
	float RGBAValue(int pos) const;
	int GradientSize() const;
	float GradientValue(int pos) const;
	float MinValue() const { return m_minValue; }
	float MaxValue() const { return m_maxValue; }
};

#endif //_GRADIENTMAPLOADER_H_
