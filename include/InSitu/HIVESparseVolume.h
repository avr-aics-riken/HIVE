#ifndef HIVE_INSITU_SPARSE_VOLUME_H__
#define HIVE_INSITU_SPARSE_VOLUME_H__

/// \brief
/// Data structure of SparseVolume for In-situ volume visualization.
///

#define HIVE_VOLUME_FORMAT_FLOAT          (0)
#define HIVE_VOLUME_FORMAT_DOUBLE         (1)
//#define HIVE_VOLUME_FORMAT_UNSIGNED_BYTE  (2) // @todo
//#define HIVE_VOLUME_FORMAT_BYTE           (3) // @todo


#ifdef __cplusplus
extern "C" {
#endif

/// \struct 
/// Volume block structure.
///
/// \note
/// Volume data is reprented in the integer coordinate.
/// Global transform(translation, rotation and scaling) is done in the scene graph level.
///
/// \note
/// `size` must be less than or equal to `extent` and `extent` should be a multiple of `size`
/// Usually, `extent` = `size` * (1 << level) 
/// For example, extent = (16, 16, 16), size = (8, 8, 8), level = 1
///
/// Global transformation(i.e. scale, translate, rotation) is applied at SceneScript level.
///
typedef struct {
  int offset[3];              ///< Block offset in local space.
  int extent[3];              ///< Block extent in local space(a multiple of size[]).
  int size[3];                ///< Cell size(Actual volume size)
  int id;                     ///< Block ID
  int level;                  ///< LoD level(0 = finest, 1, 2, ... = coarser)
  const unsigned char *data;  ///< Opaque pointer to linearlized volume data(size[0] * size[1] * size[2] * sizeof(format) * components)
} HIVEVolumeBlock;

/// \struct 
/// Represents sparse volume primitive(contains a list of volume blocks)
typedef struct {
  int numBlocks;    ///< The number of volume blocks.
  HIVEVolumeBlock *blocks;  ///< [numBlocks]
  int globalDim[3]; ///< Optional. global dim(the extent containing all volume blocks).
  int components;   ///< # of components in each voxel(up to 4).
                    ///< Assume all volume blocks have same # of components.
  int format;       ///< Voxel format(HIVE_VOLUME_FORMAT_***)
                    ///< (Assume all volume blocks have same format)
  int maxLevel;     ///< Maximum LoD level in this volume.
} HIVESparseVolume;

#ifdef __cplusplus
}
#endif

#endif // HIVE_INSITU_SPARSE_VOLUME_H__
