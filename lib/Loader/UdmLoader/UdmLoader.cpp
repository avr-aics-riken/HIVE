/**
 * @file UdmLoader.cpp
 * UDMデータローダー
 */
//
// TODO: - support various element data type
//

#ifndef HIVE_WITH_UDMLIB
#error "HIVE_WITH_UDMLIB must be defined when you compile UDMLoader module"
#endif

#include <mpi.h>

#include <stdio.h>
#include <string.h>

#include <string>
#include <fstream>

#include "UdmLoader.h"

// UDMlib headers
#include "udmlib.h"
#include "model/UdmModel.h"
#include "model/UdmZone.h"

#include "BufferMeshData.h"
#include "BufferTetraData.h"
#include "BufferSolidData.h"

/// コンストラクタ
UDMLoader::UDMLoader()
{
    Clear();
}

/// デストラクタ
UDMLoader::~UDMLoader()
{
    Clear();
}

/// クリア
void UDMLoader::Clear()
{
	m_mesh = 0;
	m_tetra = 0;
	m_pyramid = 0;
	m_prism = 0;
	m_hexa = 0;

	for (scalarArrayMap::iterator it = m_scalarArrayList.begin(); it != m_scalarArrayList.end(); it++) {
		it->second.clear();
	}

	for (vec2ArrayMap::iterator it = m_vec2ArrayList.begin(); it != m_vec2ArrayList.end(); it++) {
		it->second.clear();
	}

	for (vec3ArrayMap::iterator it = m_vec3ArrayList.begin(); it != m_vec3ArrayList.end(); it++) {
		it->second.clear();
	}

	for (vec4ArrayMap::iterator it = m_vec4ArrayList.begin(); it != m_vec4ArrayList.end(); it++) {
		it->second.clear();
	}

	for (uintArrayMap::iterator it = m_uintArrayList.begin(); it != m_uintArrayList.end(); it++) {
		it->second.clear();
	}


}

/**
 * Add solution to be loaded
 * @param name Name of soltion(e.g. Temperature, Pressure)
 */
void UDMLoader::AddSolution(const char* solutionName)
{
	SolutionInfo info;
	info.name = std::string(solutionName);
	// .type is not used here.

	m_solutions.push_back(info);
}

/**
 * UDMデータのロード
 * @param filename DFI ファイルパス
 * @param timeStepNo Timestep number
 * @retval true 成功
 * @retval false 失敗
 */
bool UDMLoader::Load(const char* filename, int timeStepNo)
{
    Clear();

    //
    // NOTE: Assume MPI_Init() was already called in hrender::main()
    //

    if (!std::ifstream(filename).good()) {
        fprintf(stderr, "[UDMLoader] File not found: %s\n", filename);
        return false;
    }
        

	udm::UdmModel* model = new udm::UdmModel();

	// Assume .dfi filename 
	if (model->loadModel(filename, timeStepNo) != UDM_OK) {
        fprintf(stderr, "[UDMLoader] Load failed: filename=%s, stepno=%d\n", filename, timeStepNo);
		delete model;
		return false;
	}

	udm::UdmDfiConfig *config = model->getDfiConfig();
	udm::UdmUnitListConfig *unitsConfig = config->getUnitListConfig();
	udm::UdmFlowSolutionListConfig *solutions = config->getFlowSolutionListConfig();

	if (unitsConfig->existsUnitConfig("Length")) {
		std::string unit;
		float reference = 0.0;
		unitsConfig->getUnit("Length", unit);
		unitsConfig->getReference("Length", reference);
		printf("[UDMLoader] Length: unit=%s, reference=%e\n", unit.c_str(), reference);
	}

	if (unitsConfig->existsUnitConfig("Pressure")) {
		std::string unit;
		float reference = 0.0;
		float difference = 0.0;
		unitsConfig->getUnit("Pressure", unit);
		unitsConfig->getReference("Pressure", reference);
		unitsConfig->getDifference("Pressure", difference);
		printf("[UDMLoader] Pressure: unit=%s, reference=%e, difference=%e\n", unit.c_str(), reference, difference);
	}

	if (unitsConfig->existsUnitConfig("Temperature")) {
		std::string unit;
		float reference = 0.0;
		float difference = 0.0;
		unitsConfig->getUnit("Temperature", unit);
		unitsConfig->getReference("Temperature", reference);
		unitsConfig->getDifference("Temperature", difference);
		printf("[UDMLoader] Temperature: unit=%s, reference=%e, difference=%e\n", unit.c_str(), reference, difference);
	}

	if (unitsConfig->existsUnitConfig("Velocity")) {
		std::string unit;
		float reference = 0.0;
		unitsConfig->getUnit("Velocity", unit);
		unitsConfig->getReference("Velocity", reference);
		printf("[UDMLoader] Velocity: unit=%s, reference=%e\n", unit.c_str(), reference);
	}

	if (unitsConfig->existsUnitConfig("Mass")) {
		std::string unit;
		float reference = 0.0;
		unitsConfig->getUnit("Mass", unit);
		unitsConfig->getReference("Mass", reference);
		printf("[UDMLoader] Mass: unit=%s, reference=%e\n", unit.c_str(), reference);
	}

	int numZones = model->getNumZones();

	if (numZones < 1) {
		fprintf(stderr, "[UDMLoader] No zones in the file.\n");
		delete model;
		return false;
	}

	std::vector<float> trianglePoints;
	std::vector<int> triangleIndices;

	std::vector<float> tetraPoints;
	std::vector<int> tetraIndices;

	std::vector<float> pyramidPoints;
	std::vector<int> pyramidIndices;

	std::vector<float> prismPoints;
	std::vector<int> prismIndices;

	std::vector<float> hexaPoints;
	std::vector<int> hexaIndices;

	std::vector<std::string> solutionNameList;
	solutions->getSolutionNameList(solutionNameList);
	printf("[UDMloader] # of solutions = %d\n", static_cast<int>(solutionNameList.size()));
	for (size_t i = 0; i < solutionNameList.size(); i++) {
		printf("  [%d] solution name = \"%s\"\n", i, solutionNameList[i].c_str()); 
		AddSolution(solutionNameList[i].c_str());
	}

	// Supported datat type: Float or Double, 1 or 3 components
	std::vector<SolutionInfo> validScalarSolutions;
	std::vector<SolutionInfo> validVec3Solutions;

	for (size_t i = 0; i < m_solutions.size(); i++) {
		if (solutions->existsSolutionConfig(m_solutions[i].name)) {
			if ((solutions->getDataType(m_solutions[i].name) == udm::Udm_RealSingle) ||
			    (solutions->getDataType(m_solutions[i].name) == udm::Udm_RealDouble)) {

				if (solutions->getVectorType(m_solutions[i].name) == udm::Udm_Scalar) {

					// Got it.
					SolutionInfo info;
					info = m_solutions[i];
					info.type = solutions->getDataType(m_solutions[i].name);
					validScalarSolutions.push_back(info);

				}
			}
		}
	}

	for (size_t i = 0; i < m_solutions.size(); i++) {
		if (solutions->existsSolutionConfig(m_solutions[i].name)) {
			if ((solutions->getDataType(m_solutions[i].name) == udm::Udm_RealSingle) ||
			    (solutions->getDataType(m_solutions[i].name) == udm::Udm_RealDouble)) {

				if 	(solutions->getVectorType(m_solutions[i].name) == udm::Udm_Vector) {

					// Got it.
					SolutionInfo info;
					info = m_solutions[i];
					info.type = solutions->getDataType(m_solutions[i].name);
					validVec3Solutions.push_back(info);

				}
			}
		}
	}


	// NOTE: ID starts with 1.
	for (int zoneID = 1; zoneID <= numZones; zoneID++) {
		const udm::UdmZone* zone = model->getZone(zoneID);
		assert(zone);

		//const udm::UdmGridCoordinates* grid = zone->getGridCoordinates();

		//const udm::UdmElements* elements = zone->getSections()->getSection(1); // @fixme

		int numNodes = zone->getNumNodes();
		int numCells = zone->getNumCells();
		//printf("zone[%d] numNodes: %d\n", zoneID, numNodes);
		//printf("zone[%d] numCells: %d\n", zoneID, numCells);

		//for (int nodeID = 1; nodeID <= numNodes; nodeID++) {
		//	const udm::UdmNode* node = zone->getNode(nodeID);
		//	double x, y, z;
		//	node->getCoords(x, y, z);
		//	udm::UdmElementType_t type = node->getElementType();
		//	printf("node[%d] type = %d, coord = %f, %f, %f\n", nodeID, type, x, y, z);
		//}

		// @todo { read phyaical value on the vertex(node) }
		for (int cellID = 1; cellID <= numCells; cellID++) {
			const udm::UdmCell* cell = zone->getCell(cellID);
			// Only supports 3D Solid type
			udm::UdmElementType_t type = cell->getElementType();
			//printf("cell[%d] type = %d, numNodes = %d\n", cellID, type, cell->getNumNodes());

			std::vector<float> points;

			for (int n = 1; n <= cell->getNumNodes(); n++) {
				const udm::UdmNode* node = cell->getNode(n);
				double x, y, z;
				node->getCoords(x, y, z);
				//printf("  [%d] = %f, %f, %f\n", n, x, y, z);

				points.push_back(static_cast<float>(x));
				points.push_back(static_cast<float>(y));
				points.push_back(static_cast<float>(z));

				// scalar solutions
				for (size_t i = 0; i < validScalarSolutions.size(); i++) {
					const std::string& name = validScalarSolutions[i].name;
					if (validScalarSolutions[i].type == udm::Udm_RealSingle) {
						float value;
						node->getSolutionScalar(name, value);

						m_scalarArrayList[name].push_back(value);

					} else if (validScalarSolutions[i].type == udm::Udm_RealDouble) {
						double value;
						node->getSolutionScalar(name, value);

						// clamp to float
						m_scalarArrayList[name].push_back(value);

					}
				}

				// vector solutions
				for (size_t i = 0; i < validVec3Solutions.size(); i++) {
					const std::string& name = validVec3Solutions[i].name;
					if (validVec3Solutions[i].type == udm::Udm_RealSingle) {
						float value[3];
						node->getSolutionVector(name, value);

						m_vec3ArrayList[name].push_back(value[0]);
						m_vec3ArrayList[name].push_back(value[1]);
						m_vec3ArrayList[name].push_back(value[2]);

					} else if (validVec3Solutions[i].type == udm::Udm_RealDouble) {
						double value[3];
						node->getSolutionVector(name, value);

						// clamp to float
						m_vec3ArrayList[name].push_back(value[0]);
						m_vec3ArrayList[name].push_back(value[1]);
						m_vec3ArrayList[name].push_back(value[2]);

					}
				}
			}

			// See UDMlib doc for the ordering of vertex index.
			if (type == udm::Udm_TRI_3) {

				size_t indexOffset = trianglePoints.size() / 3;
				for (int i = 0; i < 3; i++) {
					triangleIndices.push_back(indexOffset + i);
				}
				trianglePoints.insert(trianglePoints.end(), points.begin(), points.end());
			} else if (type == udm::Udm_QUAD_4) {

				// Convert to 2 triangles.

				size_t indexOffset = trianglePoints.size() / 3;

				triangleIndices.push_back(indexOffset + 0);
				triangleIndices.push_back(indexOffset + 1);
				triangleIndices.push_back(indexOffset + 2);

				triangleIndices.push_back(indexOffset + 0);
				triangleIndices.push_back(indexOffset + 2);
				triangleIndices.push_back(indexOffset + 3);

				trianglePoints.insert(trianglePoints.end(), points.begin(), points.end());

			} else if (type == udm::Udm_TETRA_4) {
				size_t indexOffset = tetraPoints.size() / 3;
				tetraIndices.push_back(indexOffset + 0);
				tetraIndices.push_back(indexOffset + 1);
				tetraIndices.push_back(indexOffset + 2);
				tetraIndices.push_back(indexOffset + 3);
				tetraPoints.insert(tetraPoints.end(), points.begin(), points.end());
			} else if (type == udm::Udm_PYRA_5) {
				size_t indexOffset = pyramidPoints.size() / 3;
				pyramidIndices.push_back(indexOffset + 0);
				pyramidIndices.push_back(indexOffset + 1);
				pyramidIndices.push_back(indexOffset + 2);
				pyramidIndices.push_back(indexOffset + 3);
				pyramidIndices.push_back(indexOffset + 4);
				pyramidPoints.insert(pyramidPoints.end(), points.begin(), points.end());
			} else if (type == udm::Udm_PENTA_6) {
				size_t indexOffset = prismPoints.size() / 3;
				prismIndices.push_back(indexOffset + 0);
				prismIndices.push_back(indexOffset + 1);
				prismIndices.push_back(indexOffset + 2);
				prismIndices.push_back(indexOffset + 3);
				prismIndices.push_back(indexOffset + 4);
				prismIndices.push_back(indexOffset + 5);
				prismPoints.insert(prismPoints.end(), points.begin(), points.end());
			} else if (type == udm::Udm_HEXA_8) {

#if 0 // @todo { remove. }
				// Convert hexahedron to triangle object(cube-like)
				// @todo { define hexahedron primitive }
				//
				// Hexa cube with shared vertex.
				const int idx [] = {
					0, 2, 1, 0, 3, 2,	// -Y(bottom)
					1, 2, 5, 2, 6, 5,   // +X
					5, 6, 7, 5, 7, 4,   // +Y(top)
					4, 7, 3, 4, 3, 0,   // -X
					0, 1, 5, 0, 5, 4,   // +Z
					6, 2, 3, 6, 3, 7    // -Z
				};

				size_t indexOffset = trianglePoints.size() / 3;
				for (int i = 0; i < 36; i++) {
					triangleIndices.push_back(indexOffset + idx[i]);
				}
				trianglePoints.insert(trianglePoints.end(), points.begin(), points.end());
#else
				size_t indexOffset = hexaPoints.size() / 3;
				hexaIndices.push_back(indexOffset + 0);
				hexaIndices.push_back(indexOffset + 1);
				hexaIndices.push_back(indexOffset + 2);
				hexaIndices.push_back(indexOffset + 3);
				hexaIndices.push_back(indexOffset + 4);
				hexaIndices.push_back(indexOffset + 5);
				hexaPoints.insert(hexaPoints.end(), points.begin(), points.end());
#endif
			} else {
				fprintf(stderr, "[UdmLoader] Unsupported element type: %d\n", type);
				continue;
			}
		}

	}

	//printf("trianglePoints : %d\n", trianglePoints.size());
	//printf("triangleIndices: %d\n", triangleIndices.size());
	//printf("tetraPoints : %d\n", tetraPoints.size());
	//printf("tetraIndices: %d\n", tetraIndices.size());
	
	// Get Solution data
	

	if ((trianglePoints.size() > 0) && (triangleIndices.size() > 0)) {
		int numVertices = trianglePoints.size() / 3;
		int numIndices = triangleIndices.size();
		//printf("verts = %d\n", numVertices);
		//printf("inds = %d\n", numIndices);
        m_mesh = BufferMeshData::CreateInstance();

		//for (int x = 0; x < numIndices/3; x++) {
		//	printf("inds[%d] = %d, %d, %d\n", x,
		//					triangleIndices[3*x+0],
		//					triangleIndices[3*x+1],
		//					triangleIndices[3*x+2]);
		//}

		//for (int x = 0; x < numVertices; x++) {
		//	printf("verts[%d] = %f, %f, %f\n", x,
		//					trianglePoints[3*x+0],
		//					trianglePoints[3*x+1],
		//					trianglePoints[3*x+2]);
		//}

		m_mesh->Create(numVertices, numIndices);
		Vec3Buffer* pos = m_mesh->Position();
		UintBuffer* index = m_mesh->Index();

		pos->Create(numVertices);
		memcpy(pos->GetBuffer(), &trianglePoints.at(0), sizeof(float) * 3 * numVertices);

		index->Create(numIndices);
		memcpy(index->GetBuffer(), &triangleIndices.at(0), sizeof(unsigned int) * numIndices);

		// @todo { normal, material }

	}

	if ((tetraPoints.size() > 0) && (tetraIndices.size() > 0)) {
		int numVertices = tetraPoints.size() / 3;
		int numIndices = tetraIndices.size();
        m_tetra = BufferTetraData::CreateInstance();

		m_tetra->Create(numVertices, numIndices);
		Vec3Buffer* pos = m_tetra->Position();
		UintBuffer* index = m_tetra->Index();

		pos->Create(numVertices);
		memcpy(pos->GetBuffer(), &tetraPoints.at(0), sizeof(float) * 3 * numVertices);

		index->Create(numIndices);
		memcpy(index->GetBuffer(), &tetraIndices.at(0), sizeof(unsigned int) * numIndices);

		// @todo { normal, material }
	}

	if ((pyramidPoints.size() > 0) && (pyramidIndices.size() > 0)) {

		int numVertices = pyramidPoints.size() / 3;
		int numIndices = pyramidIndices.size();
		m_pyramid = new BufferSolidData();

		m_pyramid->Create(BufferSolidData::SOLID_PYRAMID, numVertices, numIndices);
		Vec3Buffer* pos = m_pyramid->Position();
		UintBuffer* index = m_pyramid->Index();

		pos->Create(numVertices);
		memcpy(pos->GetBuffer(), &pyramidPoints.at(0), sizeof(float) * 3 * numVertices);

		index->Create(numIndices);
		memcpy(index->GetBuffer(), &pyramidIndices.at(0), sizeof(unsigned int) * numIndices);

		// @todo { normal, material }
	}

	if ((prismPoints.size() > 0) && (prismIndices.size() > 0)) {

		int numVertices = prismPoints.size() / 3;
		int numIndices = prismIndices.size();
		m_prism = new BufferSolidData();

		m_prism->Create(BufferSolidData::SOLID_PRISM, numVertices, numIndices);
		Vec3Buffer* pos = m_prism->Position();
		UintBuffer* index = m_prism->Index();

		pos->Create(numVertices);
		memcpy(pos->GetBuffer(), &prismPoints.at(0), sizeof(float) * 3 * numVertices);

		index->Create(numIndices);
		memcpy(index->GetBuffer(), &prismIndices.at(0), sizeof(unsigned int) * numIndices);

		// @todo { normal, material }
	}

	if ((hexaPoints.size() > 0) && (hexaIndices.size() > 0)) {

		int numVertices = hexaPoints.size() / 3;
		int numIndices = hexaIndices.size();
		m_hexa = new BufferSolidData();

		m_hexa->Create(BufferSolidData::SOLID_HEXAHEDRON, numVertices, numIndices);
		Vec3Buffer* pos = m_hexa->Position();
		UintBuffer* index = m_hexa->Index();

		pos->Create(numVertices);
		memcpy(pos->GetBuffer(), &hexaPoints.at(0), sizeof(float) * 3 * numVertices);

		index->Create(numIndices);
		memcpy(index->GetBuffer(), &hexaIndices.at(0), sizeof(unsigned int) * numIndices);

		// @todo { normal, material }
	}

	delete model;
	return true;
}

BufferMeshData* UDMLoader::MeshData() {
	return m_mesh;
}

BufferTetraData* UDMLoader::TetraData() {
	return m_tetra;
}

BufferSolidData* UDMLoader::SolidData(int solidType) {
	if (solidType == 5) {
		return m_pyramid;
	} else if (solidType == 6) {
		return m_prism;
	} else if (solidType == 8) {
		return m_hexa;
	}
	return NULL; // Invalid solidType specified.
}

BufferExtraData* UDMLoader::ExtraData(const char* name) {
	if (m_data.find(name) != m_data.end()) {
		return m_data[name];
	}

	// Create buffer
	if (m_scalarArrayList.find(name) != m_scalarArrayList.end()) {
        BufferExtraData* buf = BufferExtraData::CreateInstance();
		buf->Create("float", m_scalarArrayList[name].size());
		std::copy(m_scalarArrayList[name].begin(), m_scalarArrayList[name].end(), buf->Float()->GetBuffer());
		m_data[name] = buf;
		return m_data[name];
	} else if (m_vec2ArrayList.find(name) != m_vec2ArrayList.end()) {
        BufferExtraData* buf = BufferExtraData::CreateInstance();
		buf->Create("vec2", m_vec2ArrayList[name].size()/2);
		std::copy(m_vec2ArrayList[name].begin(), m_vec2ArrayList[name].end(), buf->Vec2()->GetBuffer());
		m_data[name] = buf;
		return m_data[name];
	} else if (m_vec3ArrayList.find(name) != m_vec3ArrayList.end()) {
        BufferExtraData* buf = BufferExtraData::CreateInstance();
		buf->Create("vec3", m_vec3ArrayList[name].size()/3);
		std::copy(m_vec3ArrayList[name].begin(), m_vec3ArrayList[name].end(), buf->Vec3()->GetBuffer());
		m_data[name] = buf;
		return m_data[name];
	} else if (m_vec4ArrayList.find(name) != m_vec4ArrayList.end()) {
        BufferExtraData* buf = BufferExtraData::CreateInstance();
		buf->Create("vec4", m_vec4ArrayList[name].size()/4);
		std::copy(m_vec4ArrayList[name].begin(), m_vec4ArrayList[name].end(), buf->Vec4()->GetBuffer());
		m_data[name] = buf;
		return m_data[name];
	} else if (m_uintArrayList.find(name) != m_uintArrayList.end()) {
        BufferExtraData* buf = BufferExtraData::CreateInstance();
		buf->Create("uint", m_uintArrayList[name].size());
		std::copy(m_uintArrayList[name].begin(), m_uintArrayList[name].end(), buf->Vec4()->GetBuffer());
		m_data[name] = buf;
		return m_data[name];
	}

	printf("[UdmLoader] Error: Specified solution not found \"%s\"\n", name);

	return NULL;
}

