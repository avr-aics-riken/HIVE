/**
 * @file PdmLoader.cpp
 * PDMデータローダー
 */
#include "PdmLoader.h"

#ifndef HIVE_WITH_PDMLIB
#error "HIVE_WITH_PDMLIB must be defined when you compile CDMLoader module"
#endif

#include <cstring>

#include "PDMlib.h"
#include "../Buffer/Buffer.h"

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
	m_vec3Map.clear();
	m_floatMap.clear();
	m_uintMap.clear();
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

Vec3Buffer *PDMLoader::Vec3Data(const char* containerName)
{
	if (m_vec3Map.find(containerName) != m_vec3Map.end()) {
		return m_vec3Map[containerName];
	}

	float* values = NULL;
	size_t n = 0;
	int ret = PDMlib::PDMlib::GetInstance().Read(containerName, &n, &values, &m_timeStep, m_readAll);

	if (n == 0 || values == NULL) {
		fprintf(stderr, "[CDMLoader] No such a container with point type: %s\n", containerName);
		return NULL;
	}

	printf("[PDMloader] %s : n = %d\n", containerName, n);

	// Alloc
	Vec3Buffer* buf = new Vec3Buffer();
	buf->Create(n/3);
	std::copy(values, values + n, buf->GetBuffer());

	m_vec3Map[containerName] = buf;

	delete [] values; // Release memory allocated by PDMlib 
	
	return m_vec3Map[containerName];
}

FloatBuffer *PDMLoader::FloatData(const char* containerName)
{
	if (m_floatMap.find(containerName) != m_floatMap.end()) {
		return m_floatMap[containerName];
	}

	float* values = NULL;
	size_t n = 0;
	int ret = PDMlib::PDMlib::GetInstance().Read(containerName, &n, &values, &m_timeStep, m_readAll);

	if (n == 0 || values == NULL) {
		fprintf(stderr, "[CDMLoader] No such a container with point type: %s\n", containerName);
		return NULL;
	}

	printf("[PDMloader] %s : n = %d\n", containerName, n);

	// Alloc
	FloatBuffer* buf = new FloatBuffer();
	buf->Create(n);
	std::copy(values, values + n, buf->GetBuffer());

	m_floatMap[containerName] = buf;

	delete [] values; // Release memory allocated by PDMlib 
	
	return m_floatMap[containerName];
}

UintBuffer *PDMLoader::UintData(const char* containerName)
{
	if (m_uintMap.find(containerName) != m_uintMap.end()) {
		return m_uintMap[containerName];
	}

	unsigned int * values = NULL;
	size_t n = 0;
	int ret = PDMlib::PDMlib::GetInstance().Read(containerName, &n, &values, &m_timeStep, m_readAll);

	if (n == 0 || values == NULL) {
		fprintf(stderr, "[CDMLoader] No such a container with point type: %s\n", containerName);
		return NULL;
	}

	printf("[PDMloader] %s : n = %d\n", containerName, n);

	// Alloc
	UintBuffer* buf = new UintBuffer();
	buf->Create(n);
	std::copy(values, values + n, buf->GetBuffer());

	m_uintMap[containerName] = buf;

	delete [] values; // Release memory allocated by PDMlib 
	
	return m_uintMap[containerName];
}
