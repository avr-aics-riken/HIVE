/**
 * @file VisioGradientMapLoader.h
 * VisioGradientMapデータローダー
 */
#ifndef _VISIOGRADIENTMAPLOADER_H_
#define _VISIOGRADIENTMAPLOADER_H_

#include "Ref.h"
#include "Buffer.h"

/**
 * VisioGradientMapデータローダー
 */
class VisioGradientMapLoader : public RefCount
{
public:
	VisioGradientMapLoader();
	~VisioGradientMapLoader();
	void Clear();
	RefPtr<FloatBuffer> m_rgba;
	float m_minValue;
	float m_maxValue;
protected:
	bool Load(const char* filename);
	FloatBuffer* RGBAData() { return m_rgba; }
	int RGBASize() const;
	float RGBAValue(int pos) const;
    int GradientSize() const;
    float GradientValue(int pos) const;
	float MinValue() const { return m_minValue; }
	float MaxValue() const { return m_maxValue; }
};

#endif //_VISIOGRADIENTMAPLOADER_H_
