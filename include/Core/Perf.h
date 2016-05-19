/**
 * @file Perf.h
 * Simple wrapper class for PMlib
 */
#ifndef HIVE_CORE_PROF_H__
#define HIVE_CORE_PROF_H__

#if 0
#ifdef HIVE_WITH_PMLIB
#include "PerfMonitor.h"

#define HIVE_PERF_LABEL_CORE_MAIN "Core::Main"

#define HIVE_PERF_LABEL_RENDERCORE_RENDER "RenderCore::Render"

//#define HIVE_PERF_LABEL_LOADER_CDMLOAD "Loader::BDMLoad"
#define HIVE_PERF_LABEL_LOADER_CDMLOAD "Loader::CDMLoad"
#define HIVE_PERF_LABEL_LOADER_PDMLOAD "Loader::PDMLoad"
#define HIVE_PERF_LABEL_LOADER_UDMLOAD "Loader::UDMLoad"
#define HIVE_PERF_LABEL_LOADER_HDMLOAD "Loader::HDMLoad"
#define HIVE_PERF_LABEL_LOADER_STLLOAD "Loader::STLLoad"
#define HIVE_PERF_LABEL_LOADER_OBJLOAD "Loader::OBJLoad"
#define HIVE_PERF_LABEL_LOADER_PDBLOAD "Loader::PDBLoad"
#define HIVE_PERF_LABEL_LOADER_RAWVOLUMELOAD "Loader::RawVolumeLoad"
#define HIVE_PERF_LABEL_LOADER_SPHLOAD "Loader::SPHLoad"
#define HIVE_PERF_LABEL_LOADER_VOLLOAD "Loader::VOLLoad"
#define HIVE_PERF_LABEL_LOADER_VTKLOAD "Loader::VTKLoad"

#define HIVE_PERF_LABEL_COMPOSITOR_COMPOSITE "Compositor::Composite"

#define HIVE_PERF_LABEL_ANALYZER_POLYGON "Analyzer::Polygon"
#define HIVE_PERF_LABEL_ANALYZER_VOLUME "Analyzer::Polygon"

#define HIVE_PERF_LABEL_BUILDER_MESH_TO_VOLUME "Builder::MeshToVolume"
#define HIVE_PERF_LABEL_BUILDER_MESHATTRIB_TO_VOLUME "Builder::MeshAttribToVolume"
#define HIVE_PERF_LABEL_BUILDER_POINT_TO_VOLUME "Builder::PointToVolume"
#define HIVE_PERF_LABEL_BUILDER_SPARSE_VOLUME_TO_VOLUME "Builder::SparseVolumeToVolume"
#define HIVE_PERF_LABEL_BUILDER_VOLUME_FILTER "Builder::VolumeFilter"
#define HIVE_PERF_LABEL_BUILDER_VOLUME_TO_MESHDATA "Builder::VolumeToMeshData"
#define HIVE_PERF_LABEL_BUILDER_VOLUME_TO_VECTOR "Builder::VolumeToVECTOR"

#define HIVE_PERF_LABEL_IMAGE_SAVE "Image::Save"

#define HIVE_PERF_LABEL_NETWORK_SEND "Network::Send"

#define HIVE_PERF_LABEL_SAVER_PDMSAVE "Saver::PDMSave"
#define HIVE_PERF_LABEL_SAVER_RAWVOLUMESAVE "Saver::RawVolumeSave"
#define HIVE_PERF_LABEL_SAVER_SPHSAVE "Saver::SPHSave"

typedef enum
{
  PMON_CALC,
  PMON_COMM,
} HivePMonType;

typedef struct
{
  const char* label;
  int typepm_lib::PerfMonitor::Type type;
  bool exclusive;
} HIVEPerfLabel;

const HIVEPerfLabel HIVEPerfLabels[] = {
    { HIVE_PERF_LABEL_CORE_MAIN, pm_lib::PerfMonitor::CALC, false }

  , { HIVE_PERF_LABEL_RENDERCORE_RENDER, pm_lib::PerfMonitor::CALC, false }

  , { HIVE_PERF_LABEL_LOADER_CDMLOAD, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_LOADER_PDMLOAD, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_LOADER_UDMLOAD, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_LOADER_HDMLOAD, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_LOADER_STLLOAD, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_LOADER_OBJLOAD, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_LOADER_PDBLOAD, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_LOADER_RAWVOLUMELOAD, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_LOADER_SPHLOAD, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_LOADER_VOLLOAD, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_LOADER_VTKLOAD, pm_lib::PerfMonitor::CALC, true }

  , { HIVE_PERF_LABEL_COMPOSITOR_COMPOSITE, pm_lib::PerfMonitor::CALC, true }

  , { HIVE_PERF_LABEL_ANALYZER_POLYGON, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_ANALYZER_VOLUME, pm_lib::PerfMonitor::CALC, true }

  , { HIVE_PERF_LABEL_BUILDER_MESH_TO_VOLUME, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_BUILDER_MESHATTRIB_TO_VOLUME, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_BUILDER_POINT_TO_VOLUME, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_BUILDER_SPARSE_VOLUME_TO_VOLUME, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_BUILDER_VOLUME_FILTER, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_BUILDER_VOLUME_TO_MESHDATA, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_BUILDER_VOLUME_TO_VECTOR, pm_lib::PerfMonitor::CALC, true }

  , { HIVE_PERF_LABEL_IMAGE_SAVE, pm_lib::PerfMonitor::CALC, true }

  , { HIVE_PERF_LABEL_NETWORK_SEND, pm_lib::PerfMonitor::COMM, true }

  , { HIVE_PERF_LABEL_SAVER_PDMSAVE, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_SAVER_RAWVOLUMESAVE, pm_lib::PerfMonitor::CALC, true }
  , { HIVE_PERF_LABEL_SAVER_SPHSAVE, pm_lib::PerfMonitor::CALC, true }

};

extern pm_lib::PerfMonitor& GetPM();
extern void InitPM();
extern void FinalizePM();
#endif 
#endif

///
/// Performance monitor singleton class(wrapper class for PMlib).
///
class
PMon
{
public:
  typedef enum {
    PMON_CALC,
    PMON_COMM,
  } PMonType;

  static void Init();

  ///< Register measurement point. Valid after Init().
  static bool Register(const char* label, PMonType type, bool exclusive = false);

  ///< Start measurement. `label` must be registered one in Register()
  static bool Start(const char* label);

  ///< Stop measurement. `label` must be registered one in Register()
  static bool Stop(const char* label);

  ///< Reports a profiling result.
  static void Report();

  /// Singleton interface.
  //static PMON& GetInstance();

private:
  PMon() {};

  class Impl;
  static Impl& getImpl();
};

#endif // HIVE_CORE_PROF_H__
