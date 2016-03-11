/**
 * @file PolygonAnalyzer.h
 * ポリゴンデータ解析モジュール
 */

#ifndef _POLYGONANALYZER_H_
#define _POLYGONANALYZER_H_

#include "Ref.h"
class BufferMeshData;

/**
 * ポリゴンデータ解析クラス
 */
class PolygonAnalyzer : public RefCount
{
private:
    double m_minVal[3];
    double m_maxVal[3];
    void Clear();
public:
    PolygonAnalyzer();
    ~PolygonAnalyzer();
    double MinX();
    double MinY();
    double MinZ();
    double MaxX();
    double MaxY();
    double MaxZ();
    bool Execute(BufferMeshData*);
};


#endif //_POLYGONANALYZER_LUA_H_

