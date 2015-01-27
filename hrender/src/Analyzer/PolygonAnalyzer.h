#ifndef _POLYGONANALYZER_H_
#define _POLYGONANALYZER_H_

#include "Ref.h"
#include "PolygonModel.h"

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
    bool Execute(PolygonModel*);
};


#endif //_POLYGONANALYZER_LUA_H_

