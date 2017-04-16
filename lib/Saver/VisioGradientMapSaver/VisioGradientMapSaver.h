/**
 * @file VisioGradientMapSaver.h
 * VisioGradientMapデータセーバー
 */
#ifndef _VISIOGRADIENTMAPSAVER_H_
#define _VISIOGRADIENTMAPSAVER_H_

#include "Ref.h"
#include "Buffer.h"

/**
 * VisioGradientMapデータセーバー
 */
class VisioGradientMapSaver : public RefCount
{
public:
	VisioGradientMapSaver();
	~VisioGradientMapSaver();

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

#endif //_VISIOGRADIENTMAPSAVER_H_
