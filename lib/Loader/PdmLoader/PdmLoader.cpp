/**
 * @file PdmLoader.cpp
 * PDMデータローダー
 */
#ifndef HIVE_WITH_PDMLIB
#error "HIVE_WITH_PDMLIB must be defined when you compile CDMLoader module"
#endif

#include <mpi.h> // must include mpi.h befor stdio.h for Intel MPI compiler.

#include "PdmLoader.h"

#include <cstring>

#include "PDMlib.h"
#include "../Buffer/Buffer.h"

namespace {

std::string GetType(const PDMlib::ContainerInfo& info)
{
	if (info.Type == PDMlib::uINT32) {
		if (info.nComp == 1) {
			return "uint";
		}
	} else if (info.Type == PDMlib::FLOAT) {
		if (info.nComp == 1) {
			return "float";
		} else if (info.nComp == 2) {
			return "vec2";
		} else if (info.nComp == 3) {
			return "vec3";
		} else if (info.nComp == 4) {
			return "vec4";
		}
	} else if (info.Type == PDMlib::DOUBLE) {
		// @todo {}
		//if (info.nComp == 1) {
		//	return "double";
		//} else if (info.nComp == 2) {
		//	return "dvec2";
		//} else if (info.nComp == 3) {
		//	return "dvec3";
		//} else if (info.nComp == 4) {
		//	return "dvec4";
		//}
	}

	return "UNKNOWN"; // unsupported type.
}

const PDMLoader::ContainerInfo* FindContainer(const std::vector<PDMLoader::ContainerInfo> &infos, const std::string& name) {
	for (size_t i = 0; i < infos.size(); i++) {
		if (infos[i].name == name) {
			return &infos[i];
		}
	}

	return NULL;
}

} // namespace

/// コンストラクタ
PDMLoader::PDMLoader(){}
/// デストラクタ
PDMLoader::~PDMLoader(){};

/// メンバクリア
void PDMLoader::Clear()
{
	m_timeStep = -1;
	m_readAll = false;
	m_pointMap.clear();
	m_containerMap.clear();
}

/**
 * PDMデータのロード
 * @param filename ファイルパス
 * @param timeStep time step
 * @param readlAll read all data? true:for data conversion, false: each MPI rank reads each paticle data.
 * @retval true 成功
 * @retval false 失敗
 */
bool PDMLoader::Load(const char* filename, int timeStep){
	Clear();

	std::string fname = std::string(filename);

	// Its OK to pass argc=0,argv=NULL since its only used to initialize MPI inside of Zoltan lib(called from PDMlib::Init()).
	// We alreadly initialized MPI, thus argc and argv will not be used.
	PDMlib::PDMlib::GetInstance().Init(0, NULL, /* write filename = */"",  fname);

	std::vector<PDMlib::ContainerInfo>& infos = PDMlib::PDMlib::GetInstance().GetContainerInfo();
	printf("[PDMloader] # of infos = %d\n", infos.size());
	for (size_t i = 0; i < infos.size(); i++) {
		printf("[PDMLoader] [%d] name = %s, type = %d, ncomp = %d, order = %d, suffix = %s, annotation = %s, compression = %s\n",
				i, infos[i].Name.c_str(), infos[i].Type, infos[i].nComp, infos[i].VectorOrder, infos[i].Suffix.c_str(), infos[i].Annotation.c_str(), infos[i].Compression.c_str());

		std::string type = GetType(infos[i]); 
		if (type != "UNKNOWN") {
			ContainerInfo containerInfo;
			containerInfo.name = infos[i].Name;
			containerInfo.type = type;
			m_containerInfoList.push_back(containerInfo);

			printf("[PDMLoader] Add container info: name = %s, type = %s\n",
					containerInfo.name.c_str(), containerInfo.type.c_str());
		}
	}

	m_timeStep = timeStep;
	m_readAll = false; // @fixme

	// NOTE: Reading attributes are done with subsequence call of ReadFloat(), ReadVec3(), ...

	return true;
}

/**
 * 点データ取得
 * @retval BufferPointData* 点データバッファへの参照
 * @param containerName name of point data container
 * @param radius Radius of point
 */
BufferPointData *PDMLoader::PointData(const char* containerName, float radius)
{
	if (m_pointMap.find(containerName) != m_pointMap.end()) {
		return m_pointMap[containerName];
	}

	float* points = NULL;
	size_t n = 0;
	int ret = PDMlib::PDMlib::GetInstance().Read(containerName, &n, &points, &m_timeStep, m_readAll);

	if (n == 0 || points == NULL) {
		fprintf(stderr, "[CDMLoader] No such a container with point type: %s\n", containerName);
		return NULL;
	}

	printf("[PDMloader] %s : n = %d\n", containerName, n);

	// Alloc
	BufferPointData* buf = new BufferPointData();
	buf->Create(n/3);
	std::copy(points, points + n, buf->Position()->GetBuffer());

	printf("[PDMloader] radius = %f\n", radius);

	std::fill(buf->Radius()->GetBuffer(), buf->Radius()->GetBuffer() + (n/3), radius);

	m_pointMap[containerName] = buf;

	delete [] points; // Release memory allocated by PDMlib 
	
	return m_pointMap[containerName];
}

BufferExtraData *PDMLoader::ExtraData(const char* containerName)
{
	if (m_containerMap.find(containerName) != m_containerMap.end()) {
		return m_containerMap[containerName];
	}

	const ContainerInfo* info = FindContainer(m_containerInfoList, containerName);
	if (info == NULL) {
		fprintf(stderr, "[CDMLoader] No such a container: %s\n", containerName);
		return NULL;
	}

	if (info->type == "float" || info->type == "vec2" || info->type == "vec3" || info->type == "vec4") {
		float* values = NULL;
		size_t n = 0;
		int ret = PDMlib::PDMlib::GetInstance().Read(containerName, &n, &values, &m_timeStep, m_readAll);

		if (n == 0 || values == NULL) {
			fprintf(stderr, "[CDMLoader] No such a container with point type: %s\n", containerName);
			return NULL;
		}

		printf("[PDMloader] %s : n = %d\n", containerName, n);

		int numElems = n;
		if (info->type == "vec2") {
			numElems /= 2;
		} else if (info->type == "vec3") {
			numElems /= 3;
		} else if (info->type == "vec4") {
			numElems /= 4;
		}

		// Alloc
		BufferExtraData* buf = new BufferExtraData();
		buf->Create(info->type.c_str(), numElems);

		if (info->type == "float") {
			std::copy(values, values + n, buf->Float()->GetBuffer());
		} else if (info->type == "vec2") {
			std::copy(values, values + n, buf->Vec2()->GetBuffer());
		} else if (info->type == "vec3") {
			std::copy(values, values + n, buf->Vec3()->GetBuffer());
		} else if (info->type == "vec4") {
			std::copy(values, values + n, buf->Vec4()->GetBuffer());
		}

		m_containerMap[containerName] = buf;

		delete [] values; // Release memory allocated by PDMlib 

		return m_containerMap[containerName];
	} else if (info->type == "uint") {
		unsigned int* values = NULL;
		size_t n = 0;
		int ret = PDMlib::PDMlib::GetInstance().Read(containerName, &n, &values, &m_timeStep, m_readAll);

		if (n == 0 || values == NULL) {
			fprintf(stderr, "[CDMLoader] No such a container with point type: %s\n", containerName);
			return NULL;
		}

		printf("[PDMloader] %s : n = %d\n", containerName, n);

		int numElems = n;

		// Alloc
		BufferExtraData* buf = new BufferExtraData();
		buf->Create(info->type.c_str(), numElems);

		std::copy(values, values + n, buf->Uint()->GetBuffer());

		m_containerMap[containerName] = buf;

		delete [] values; // Release memory allocated by PDMlib 

		return m_containerMap[containerName];
	}

	fprintf(stderr, "[CDMLoader] Unknown or unsupported container: %s\n", containerName);

	return NULL;
	
}
