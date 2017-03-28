/**
 * @file VisioColorMapLoader.h
 * VisioColorMapデータローダー
 */
#ifndef _VISIOCOLORMAPLOADER_H_
#define _VISIOCOLORMAPLOADER_H_

#include "Buffer.h"
#include "Ref.h"

/**
 * VisioColorMapデータローダー
 */
class VisioColorMapLoader : public RefCount
{
  public:
	VisioColorMapLoader();
	~VisioColorMapLoader();
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

#endif //_VISIOCOLORMAPLOADER_H_
