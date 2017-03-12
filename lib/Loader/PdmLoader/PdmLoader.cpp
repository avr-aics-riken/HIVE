/**
 * @file PdmLoader.cpp
 * PDMデータローダー
 */
#ifndef HIVE_WITH_PDMLIB
#error "HIVE_WITH_PDMLIB must be defined when you compile PDMLoader module"
#endif

#include <mpi.h> // must include mpi.h befor stdio.h for Intel MPI compiler.

#include "PdmLoader.h"

#include <cstring>

#include "../Buffer/Buffer.h"
#include "PDMlib.h"

namespace
{

// Convert to HIVE Buffer type name.
static std::string GetTypeName(const PDMlib::ContainerInfo &info)
{
	if (info.Type == PDMlib::uINT32)
	{
		if (info.nComp == 1)
		{
			return "uint";
		}
	}
	else if (info.Type == PDMlib::FLOAT)
	{
		if (info.nComp == 1)
		{
			return "float";
		}
		else if (info.nComp == 2)
		{
			return "vec2";
		}
		else if (info.nComp == 3)
		{
			return "vec3";
		}
		else if (info.nComp == 4)
		{
			return "vec4";
		}
	}
	else if (info.Type == PDMlib::DOUBLE)
	{
		if (info.nComp == 1)
		{
			return "double";
		}
		else if (info.nComp == 2)
		{
			return "dvec2";
		}
		else if (info.nComp == 3)
		{
			return "dvec3";
		}
		else if (info.nComp == 4)
		{
			return "dvec4";
		}
	}

	return "UNKNOWN"; // unsupported type.
}

static PDMLoader::Type GetType(const PDMlib::ContainerInfo &info)
{
	if (info.Type == PDMlib::uINT32)
	{
		return PDMLoader::PDM_TYPE_INT;
	}
	else if (info.Type == PDMlib::FLOAT)
	{
		return PDMLoader::PDM_TYPE_FLOAT;
	}
	else if (info.Type == PDMlib::DOUBLE)
	{
		return PDMLoader::PDM_TYPE_DOUBLE;
	}
	else
	{
		assert(0);
	}
	return PDMLoader::PDM_TYPE_INVALID;
}

const PDMLoader::ContainerInfo *
FindContainer(const std::vector<PDMLoader::ContainerInfo> &infos,
			  const std::string &name)
{
	for (size_t i = 0; i < infos.size(); i++)
	{
		if (infos[i].name == name)
		{
			return &infos[i];
		}
	}

	return NULL;
}

static int RegisterContainer(PDMlib::PDMlib &instance,
							 PDMLoader::ContainerInfo *info /*inout */)
{
	if (info->type == PDMLoader::PDM_TYPE_INT)
	{
		return instance.RegisterContainer(info->name, &(info->data_ptr.i_ptr));
	}
	else if (info->type == PDMLoader::PDM_TYPE_FLOAT)
	{
		return instance.RegisterContainer(info->name, &(info->data_ptr.f_ptr));
	}
	else if (info->type == PDMLoader::PDM_TYPE_DOUBLE)
	{
		return instance.RegisterContainer(info->name, &(info->data_ptr.d_ptr));
	}
	else
	{
		assert(0); // Unknown data type.
		return -1;
	}

	return -1;
}

} // namespace

/// コンストラクタ
PDMLoader::PDMLoader() {}
/// デストラクタ
PDMLoader::~PDMLoader(){};

/// メンバクリア
void PDMLoader::Clear()
{
	m_pointMap.clear();
	m_containerMap.clear();
	m_containerInfoList.clear();
	m_initialized = false;
	m_coordianteName = "Coordinate";
}

/// Convert read coordinate data to HIVE buffer and cache it.
bool PDMLoader::CachePoints(const char *containerName)
{
	if (!m_initialized)
	{
		fprintf(stderr, "[PDMLoader] Failed to init PDMloader\n");
		return false;
	}

	if (m_numData < 1)
	{
		fprintf(stderr, "[PDMLoader] Invalid data size.\n");
		return false;
	}

	const ContainerInfo *info =
		FindContainer(m_containerInfoList, containerName);
	if (info == NULL)
	{
		fprintf(stderr, "[PDMLoader] No such a container for coordinate: %s\n",
				containerName);
		return false;
	}

	bool valid = false;

	if ((info->type == PDMLoader::PDM_TYPE_FLOAT) ||
		(info->type == PDMLoader::PDM_TYPE_FLOAT))
	{
		if (info->n == 3)
		{
			valid = true;
		}
	}

	if (!valid)
	{
		fprintf(stderr, "[PDMLoader] Invalid data type or ncomp for "
						"coordinate. type = %d, ncomp = %d(3 expected)\n",
				info->type, info->n);
		return false;
	}

	// Alloc
	BufferPointData *buf = BufferPointData::CreateInstance();
	buf->Create(m_numData);
	// position
	size_t n = m_numData * 3;

	if (info->type == PDMLoader::PDM_TYPE_FLOAT)
	{
		assert(info->data_ptr.f_ptr);
		if (!info->data_ptr.f_ptr)
		{
			return false;
		}
		std::copy(info->data_ptr.f_ptr, info->data_ptr.f_ptr + n,
				  buf->Position()->GetBuffer());
	}
	else if (info->type == PDMLoader::PDM_TYPE_DOUBLE)
	{
		assert(info->data_ptr.d_ptr);
		if (!info->data_ptr.d_ptr)
		{
			return false;
		}
		// TODO(IDS): Support double-precision point data as a native type.
		float *dst = buf->Position()->GetBuffer();
		for (size_t i = 0; i < n; i++)
		{
			dst[i] = float(info->data_ptr.d_ptr[i]);
		}
	}
	else
	{
		assert(0);
	}

	// radius will be modified at `PointData` function. Fill it with 1.0 as a
	// default value.
	std::fill(buf->Radius()->GetBuffer(),
			  buf->Radius()->GetBuffer() + buf->Radius()->GetNum(), 1);
	m_pointMap[containerName] = buf;

	// TODO(IDS): Delete coordiante memory allocated by PDMlib here.

	return true;
}

/// Convert read non-coordinate data to HIVE buffer and cache it.
bool PDMLoader::CacheExtraData(const char *containerName)
{
	if (!m_initialized)
	{
		// Failed to init
		fprintf(stderr, "[PDMLoader] Failed to init PDMloader\n");
		return false;
	}

	const ContainerInfo *info =
		FindContainer(m_containerInfoList, containerName);
	if (info == NULL)
	{
		fprintf(stderr, "[PDMLoader] No such a container: %s\n", containerName);
		return false;
	}

	if (info->type == PDMLoader::PDM_TYPE_FLOAT)
	{
		assert(info->data_ptr.f_ptr);

		// Alloc
		int numElems = m_numData;
		assert((info->n >= 1) && (info->n <= 4));

		const size_t n = numElems * info->n;

		BufferExtraData *buf = BufferExtraData::CreateInstance();
		buf->Create(info->type_name.c_str(), numElems);

		const float *ptr = info->data_ptr.f_ptr;

		if (info->n == 1)
		{ // float
			std::copy(ptr, ptr + n, buf->Float()->GetBuffer());
		}
		else if (info->n == 2)
		{ // vec2
			std::copy(ptr, ptr + n, buf->Vec2()->GetBuffer());
		}
		else if (info->n == 3)
		{ // vec3
			std::copy(ptr, ptr + n, buf->Vec3()->GetBuffer());
		}
		else if (info->n == 4)
		{ // vec4
			std::copy(ptr, ptr + n, buf->Vec4()->GetBuffer());
		}

		m_containerMap[containerName] = buf;

		// TODO(IDS): Release memory allocated by PDMlib here.

		return true;
	}
	else if (info->type == PDMLoader::PDM_TYPE_DOUBLE)
	{
		fprintf(
			stderr,
			"[PDMLoader] Container with double type is TODO. container = %s\n",
			containerName);
		return false;
	}
	else if (info->type == PDMLoader::PDM_TYPE_INT)
	{
		assert(info->data_ptr.i_ptr);

		if (info->n != 1)
		{
			fprintf(stderr, "[PDMLoader] Currently only scalar int type is "
							"supported. container: %s\n",
					containerName);
			return false;
		}

		// Alloc
		int numElems = m_numData;

		const size_t n = numElems * info->n;

		BufferExtraData *buf = BufferExtraData::CreateInstance();
		buf->Create(info->type_name.c_str(), numElems);

		const int *ptr = info->data_ptr.i_ptr;

		if (info->n == 1)
		{ // uint
			std::copy(ptr, ptr + n, buf->Uint()->GetBuffer());
		}
		else if (info->n == 2)
		{			   // vec2
			assert(0); // TODO
		}
		else if (info->n == 3)
		{			   // vec3
			assert(0); // TODO
		}
		else if (info->n == 4)
		{			   // vec4
			assert(0); // TODO
		}

		m_containerMap[containerName] = buf;

		// TODO(IDS): Release memory allocated by PDMlib here.

		return true;
	}

	fprintf(stderr, "[PDMLoader] Unknown or unsupported container: %s\n",
			containerName);

	return false;
}

bool PDMLoader::EnableProfiling(bool onoff)
{
	if (m_initialized)
	{
		// Must be called before initialiing PDMlib.
		return false;
	}

	m_profiling = onoff;

	return true;
}

/**
 * PDMデータのロード
 * @param filename ファイルパス
 * @param timeStep time step
 * @param coordinateName Name of coordinate container
 * @param migration Do migration of loading data(for MxN parallel data loading).
 * @retval true 成功
 * @retval false 失敗
 */
bool PDMLoader::Load(const char *filename, int timeStep,
					 const char *coordinateName, bool migration)
{
	Clear();

	std::string fname = std::string(filename);

	int rank = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0)
	{
		printf("[PDMloader] Initializing PDMLib: filename = %s, timeStep = %d, "
			   "coordinateName = %s, migration = %d, profile = %d\n",
			   filename, timeStep, coordinateName, migration, m_profiling);
	}

	// Its OK to pass argc=0,argv=NULL since its only used to initialize MPI
	// inside of Zoltan lib(called from PDMlib::Init()).
	// We alreadly initialized MPI, thus argc and argv will not be used.
	PDMlib::PDMlib::GetInstance().Init(/* argc */ 0, /* argv */ NULL,
									   /* write filename = */ "",
									   /* read filename */ fname, m_profiling);

	std::vector<PDMlib::ContainerInfo> &infos =
		PDMlib::PDMlib::GetInstance().GetContainerInfo();

	if (rank == 0)
	{
		printf("[PDMloader] # of infos = %d\n", int(infos.size()));
	}

	bool hasCoordinate = false;
	m_coordianteName = coordinateName ? std::string(coordinateName) : "";

	for (size_t i = 0; i < infos.size(); i++)
	{
		if (rank == 0)
		{
			printf("[PDMLoader] [%d] name = %s, type = %d, ncomp = %d, order = "
				   "%d, "
				   "suffix = %s, annotation = %s, compression = %s\n",
				   int(i), infos[i].Name.c_str(), infos[i].Type, infos[i].nComp,
				   infos[i].VectorOrder, infos[i].Suffix.c_str(),
				   infos[i].Annotation.c_str(), infos[i].Compression.c_str());
		}

		std::string type = GetTypeName(infos[i]);
		if (type != "UNKNOWN")
		{
			ContainerInfo containerInfo;
			containerInfo.name = infos[i].Name;
			containerInfo.type_name = type;
			containerInfo.type = GetType(infos[i]);
			containerInfo.n = infos[i].nComp;

			containerInfo.data_ptr.i_ptr = NULL;
			containerInfo.data_ptr.f_ptr = NULL;
			containerInfo.data_ptr.d_ptr = NULL;

			m_containerInfoList.push_back(containerInfo);

			if (rank == 0)
			{
				printf("[PDMLoader] Add container info: name = %s, type = %s\n",
					   containerInfo.name.c_str(),
					   containerInfo.type_name.c_str());
			}

			if ((!m_coordianteName.empty()) &&
				infos[i].Name.compare(m_coordianteName) == 0)
			{
				hasCoordinate = true;
			}
		}
	}

	if (!hasCoordinate)
	{
		if (m_coordianteName.empty())
		{
			fprintf(stdout, "[PDMLoader] Null for Coordinate container.\n");
		}
		else
		{
			fprintf(stdout, "[PDMLoader] Coordinate container: %s not found.\n",
					m_coordianteName.c_str());
		}
	}

	// Read all container data.
	// TODO(IDS): Delay reading each container.
	{

		for (size_t i = 0; i < m_containerInfoList.size(); i++)
		{
			int ret = RegisterContainer(PDMlib::PDMlib::GetInstance(),
										&(m_containerInfoList[i]));
			if (ret != 0)
			{
				fprintf(stderr, "[PDMLoader] Failed to register container [ %s "
								"]. ret = %d.\n",
						m_containerInfoList[i].name.c_str(), ret);
				return false;
			}
		}

		// When Coordinate container does not exit, migration doesn't work.
		const bool doMigration = hasCoordinate ? migration : false;

		if (!hasCoordinate && migration)
		{
			fprintf(stdout, "[PDMLoader] WARN: migration disabled since "
							"Coordiante container does not exit in PDM "
							"data.\n");
		}

		size_t num_data =
			PDMlib::PDMlib::GetInstance().ReadAll(&timeStep, doMigration);

		// Ensure all containers are read.
		if (num_data < 1)
		{
			fprintf(stderr, "[PDMLoader] Invalid data size: %d.\n",
					int(num_data));
			// May OK.
		}
		m_numData = num_data;
	}

	// Converting read data to HIVE buffer will be done in subsequent call of
	// `PointData` or `ExtraData`

	m_initialized = true;

	return true;
}

/**
 * 点データ取得
 * @retval BufferPointData* 点データバッファへの参照
 * @param containerName name of point data container
 * @param radius Radius of point
 */
BufferPointData *PDMLoader::PointData(const char *containerName, float radius)
{
	// First check cache.
	if (m_pointMap.find(containerName) != m_pointMap.end())
	{
		BufferPointData *buf = m_pointMap[containerName];
		printf("[PDMloader] radius = %f\n", radius);
		std::fill(buf->Radius()->GetBuffer(),
				  buf->Radius()->GetBuffer() + buf->Radius()->GetNum(), radius);
		return buf;
	}

	// Check if container is registered but data is not yet converted to HIVE
	// buffer.
	{
		const ContainerInfo *info =
			FindContainer(m_containerInfoList, containerName);
		if (info)
		{
			bool ret = CachePoints(containerName);
			if (!ret)
			{
				return NULL;
			}

			// Usually this should be true.
			if (m_pointMap.find(containerName) != m_pointMap.end())
			{
				BufferPointData *buf = m_pointMap[containerName];
				printf("[PDMloader] radius = %f\n", radius);
				std::fill(buf->Radius()->GetBuffer(),
						  buf->Radius()->GetBuffer() + buf->Radius()->GetNum(),
						  radius);
				return buf;
			}
		}
	}

	// Container not found.
	return NULL;
}

BufferExtraData *PDMLoader::ExtraData(const char *containerName)
{
	// First check cache.
	if (m_containerMap.find(containerName) != m_containerMap.end())
	{
		return m_containerMap[containerName];
	}

	// Check if container is registered but data is not yet converted to HIVE
	// buffer.
	{
		const ContainerInfo *info =
			FindContainer(m_containerInfoList, containerName);
		if (info)
		{
			bool ret = CacheExtraData(containerName);
			if (!ret)
			{
				return NULL;
			}

			// Usually this should be true.
			if (m_containerMap.find(containerName) != m_containerMap.end())
			{
				BufferExtraData *buf = m_containerMap[containerName];
				return buf;
			}
		}
	}

	// Container not found.
	return NULL;
}
