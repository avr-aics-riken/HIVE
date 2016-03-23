/**
 * @file UdmLoader.h
 * UDMデータローダー
 */
#ifndef _UDMLOADER_H_
#define _UDMLOADER_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferExtraData.h"

#include <map>

class BufferMeshData;
class BufferTetraData;
class BufferSolidData;

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
	typedef std::map<std::string, std::vector<float> > vec2ArrayMap;
	typedef std::map<std::string, std::vector<float> > vec3ArrayMap;
	typedef std::map<std::string, std::vector<float> > vec4ArrayMap;
	typedef std::map<std::string, std::vector<unsigned int> > uintArrayMap;

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

	/// Retrieve solid primitive.
	/// solidType 5 = pyramid, 6 = prism or 8 = hexa.
	/// Valid after `Load()`
	/// NULL if the file does not contain corresponding solid primitive. 
	BufferSolidData* SolidData(int solidType);

	/// Get solution(custom attribute) data.
	BufferExtraData* ExtraData(const char *name);

private:
    std::vector<unsigned int> m_timeSteps;

	RefPtr<BufferMeshData> m_mesh;
	RefPtr<BufferTetraData> m_tetra;
	RefPtr<BufferSolidData> m_pyramid;
	RefPtr<BufferSolidData> m_prism;
	RefPtr<BufferSolidData> m_hexa;

	std::vector<SolutionInfo> m_solutions;

	scalarArrayMap m_scalarArrayList;
	vec2ArrayMap m_vec2ArrayList;
	vec3ArrayMap m_vec3ArrayList;
	vec4ArrayMap m_vec4ArrayList;
	uintArrayMap m_uintArrayList;

	std::map<std::string, RefPtr<BufferExtraData> > m_data;

};

#endif //_UDMLOADER_H_
