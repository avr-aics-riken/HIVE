/**
 * @file GradientMapSaver.h
 * GradientMapデータセーバー
 */
#ifndef _GRADIENTMAPSAVER_H_
#define _GRADIENTMAPSAVER_H_

#include "Ref.h"
#include "Buffer.h"

/**
 * GradientMapデータセーバー
 */
class GradientMapSaver : public RefCount
{
public:
	GradientMapSaver();
	~GradientMapSaver();

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

#endif //_GRADIENTMAPSAVER_H_
