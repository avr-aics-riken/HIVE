#ifndef _HIVE_PDMSAVER_H_
#define _HIVE_PDMSAVER_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferPointData.h"

class PDMSaver : public RefCount
{
public:
    PDMSaver();
    ~PDMSaver();
    bool Save(const char* filename);


    void SetPointData(const BufferPointData* pointData) {
      m_pointData = pointData;
    }

    void SetNumPoints(size_t num);

private:

    const BufferPointData* m_pointData;
    size_t                 m_numPoints;

};

#endif //_HIVE_PDMSAVER_H_

