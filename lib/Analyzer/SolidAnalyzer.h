/**
 * @file SolidAnalyzer.h
 * Solidデータ解析モジュール
 */

#ifndef _SOLIDANALYZER_H_
#define _SOLIDANALYZER_H_

#include "Ref.h"
class BufferSolidData;

/**
 * Solid データ解析クラス
 */
class SolidAnalyzer : public RefCount
{
private:
    double m_minVal[3];
    double m_maxVal[3];
    void Clear();
public:
    SolidAnalyzer();
    ~SolidAnalyzer();
    double MinX();
    double MinY();
    double MinZ();
    double MaxX();
    double MaxY();
    double MaxZ();
    bool Execute(BufferSolidData*);
};


#endif //_SOLIDANALYZER_LUA_H_

