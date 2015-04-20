/**
 * @file UdmLoader.cpp
 * UDMデータローダー
 */
//
// TODO: - timestep support
//       - support various volume data type
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

	// @todo {}
	delete model;
	return false;

}


