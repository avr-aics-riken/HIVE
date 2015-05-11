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
private:
	typedef struct {
		std::string name;
		int type;
	} SolutionInfo;

	typedef std::map<std::string, std::vector<float> > scalarArrayMap;
	typedef std::map<std::string, std::vector<float> > vec3ArrayMap;

public:
    UDMLoader();
    ~UDMLoader();
    void Clear();

	/// Add solution(physical quantity) data to be loaded as a varying data
	/// Must be called before `Load()`
	void AddSolution(const char* name);

	/// Clear solution information added by `AddSolution`.
	void ClearSolution() {
		m_solutions.clear();
	}

	/// Loads UDMlib data.
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

	/// Get vec3 solution data.
	Vec3Buffer* Vec3Data(const char* name);

	/// Get scalar solution data.
	FloatBuffer* ScalarData(const char* name);

private:
    std::vector<unsigned int> m_timeSteps;

	RefPtr<BufferMeshData> m_mesh;
	RefPtr<BufferTetraData> m_tetra;

	std::vector<SolutionInfo> m_solutions;

	scalarArrayMap m_scalarArrayList;
	vec3ArrayMap m_vec3ArrayList;

};

#endif //_UDMLOADER_H_

