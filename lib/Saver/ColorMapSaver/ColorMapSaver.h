/**
 * @file ColorMapSaver.h
 * ColorMapデータローダー
 */
#ifndef _COLORMAPSAVER_H_
#define _COLORMAPSAVER_H_

#include "Ref.h"
#include "Buffer.h"

/**
 * ColorMapデータローダー
 */
class ColorMapSaver : public RefCount
{
public:
	ColorMapSaver();
	~ColorMapSaver();
	
	RefPtr<FloatBuffer> m_rgba;
	float m_minValue;
	float m_maxValue;

protected:
	bool Save(const char* filename);

	void Clear();
	int RGBASize() const;
	bool SetRGBAValue(int pos, float value);
	bool SetMinValue(float minValue) { m_minValue = minValue; return true; }
	bool SetMaxValue(float maxValue) { m_maxValue = maxValue; return true; }
};

#endif //_COLORMAPSAVER_H_
