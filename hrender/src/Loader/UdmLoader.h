/**
 * @file CdmLoader.h
 * UDMデータローダー
 */
#ifndef _UDMLOADER_H_
#define _UDMLOADER_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

class BufferMeshData;
class BufferTetraData;

/**
 * UDMデータローダー
 */
class UDMLoader : public RefCount
{
public:
    UDMLoader();
    ~UDMLoader();
    void Clear();
    bool Load(const char* filename, int stepno);

    int NumTimeSteps() {
      return m_timeSteps.size();
    }

    ///< Get i'th timestep
    int GetTimeStep(int i) {
      if (i < m_timeSteps.size()) {
        return m_timeSteps[i];
      }
      return 0;
    }

	/// Retrieve triangle mesh geometry.
	/// Valid after `Load()`
	/// NULL if the file does not contain non-tetra primitive. 
	BufferMeshData* MeshData();

	/// Retrieve tetra geometry.
	/// Valid after `Load()`
	/// NULL if the file does not contain tetra primitive. 
	BufferTetraData* TetraData();

private:
    std::vector<unsigned int> m_timeSteps;

	RefPtr<BufferMeshData> m_mesh;
	RefPtr<BufferTetraData> m_tetra;

};

#endif //_UDMLOADER_H_

