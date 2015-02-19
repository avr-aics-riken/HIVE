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
      printf("setpointdata : %p\n", pointData);
      m_pointData = pointData;
    }

private:

    const BufferPointData* m_pointData;

};

#endif //_HIVE_PDMSAVER_H_

