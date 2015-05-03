/**
 * @file UdmLoader.cpp
 * UDMデータローダー
 */
//
// TODO: - timestep support
//       - support various element data type
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

	// NOTE: ID starts with 1.
	for (int zoneID = 1; zoneID <= numZones; zoneID++) {
		const udm::UdmZone* zone = model->getZone(zoneID);
		assert(zone);

		//const udm::UdmGridCoordinates* grid = zone->getGridCoordinates();

		//const udm::UdmElements* elements = zone->getSections()->getSection(1); // @fixme

		int numNodes = zone->getNumNodes();
		int numCells = zone->getNumCells();
		printf("zone[%d] numNodes: %d\n", zoneID, numNodes);
		printf("zone[%d] numCells: %d\n", zoneID, numCells);

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
			}

			// See UDMlib doc for the ordering of vertex index.
			if (type == udm::Udm_TRI_3) {

				assert(0); // @todo
			} else if (type == udm::Udm_QUAD_4) {
				assert(0); // @todo
			} else if (type == udm::Udm_TETRA_4) {
				size_t indexOffset = tetraPoints.size() / 3;
				tetraIndices.push_back(indexOffset + 0);
				tetraIndices.push_back(indexOffset + 1);
				tetraIndices.push_back(indexOffset + 2);
				tetraIndices.push_back(indexOffset + 3);
				tetraPoints.insert(tetraPoints.end(), points.begin(), points.end());
			} else if (type == udm::Udm_HEXA_8) {

				// Convert hexahedron to triangle object(cube-like)
				// @todo { define hexahedron primitive }
				//
				// Hexa cube with shared vertex.
				const int idx [] = {
					0, 2, 1, 1, 2, 3,	// -Y(bottom)
					1, 3, 5, 3, 7, 5,   // +X
					5, 7, 6, 5, 6, 4,   // +Y(top)
					4, 6, 2, 4, 2, 0,   // -X
					0, 1, 5, 0, 5, 4,   // +Z
					6, 7, 3, 6, 3, 2    // -Z
				};

				size_t indexOffset = trianglePoints.size() / 3;
				for (int i = 0; i < 36; i++) {
					triangleIndices.push_back(indexOffset + i);
				}
				trianglePoints.insert(trianglePoints.end(), points.begin(), points.end());
			} else {
				fprintf(stderr, "[UdmLoader] Unsupported element type: %d\n", type);
				continue;
			}
		}

	}

	printf("trianglePoints : %d\n", trianglePoints.size());
	printf("triangleIndices: %d\n", triangleIndices.size());
	printf("tetraPoints : %d\n", tetraPoints.size());
	printf("tetraIndices: %d\n", tetraIndices.size());

	if ((trianglePoints.size() > 0) && (triangleIndices.size() > 0)) {
		int numVertices = trianglePoints.size() / 3;
		int numIndices = triangleIndices.size();
		m_mesh = new BufferMeshData();

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
		// @todo
		m_tetra = new BufferTetraData();

		int numVertices = tetraPoints.size() / 3;
		int numIndices = tetraIndices.size();
		m_tetra = new BufferTetraData();

		m_tetra->Create(numVertices, numIndices);
		Vec3Buffer* pos = m_tetra->Position();
		UintBuffer* index = m_tetra->Index();

		pos->Create(numVertices);
		memcpy(pos->GetBuffer(), &tetraPoints.at(0), sizeof(float) * 3 * numVertices);

		index->Create(numIndices);
		memcpy(index->GetBuffer(), &tetraIndices.at(0), sizeof(unsigned int) * numIndices);

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

