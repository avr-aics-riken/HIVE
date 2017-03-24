/**
 * @file VisioColorMapSaver.h
 * VisioColorMapデータローダー
 */
#ifndef _VISIOCOLORMAPSAVER_H_
#define _VISIOCOLORMAPSAVER_H_

#include "Ref.h"
#include "Buffer.h"

/**
 * VisioColorMapデータローダー
 */
class VisioColorMapSaver : public RefCount
{
public:
	VisioColorMapSaver();
	~VisioColorMapSaver();
	
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

#endif //_VISIOCOLORMAPSAVER_H_
