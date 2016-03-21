# In-Situ interface

## Overview

HIVE supports In-Situ visualization through dedicated memory-based data structure and memory object API in SceneScript.


    
     +------------------+         +============+
     | User application |  ---->  | DataStruct |
     +------------------+         +============+
        read  || write                    |
     +==================+                 |
     |                  |                 |
     |      memory      |                 |
     |                  | ----+           |/
     +==================+     |     +----------------------+
                              +---->| SceneScript/HiveCore |
                           read     + ---------------------+
                                          

## Data structure

Data structure is defined in C which is used both for user application(C/C++) and HiveCore are defined in `include/InSitu` directory.

Actual data(e.g. volume data) is passed using C/C++ pointer, thus no intermediate buffer is required for In-Situ visualization.  

## Supported data type.

### RawSparseVolume

Data strucure is defined in `HIVESparseVolume.h`

```
typedef struct {
  int offset[3];              ///< Block offset in local space.
  int extent[3];              ///< Block extent in local space(a multiple of size[]).
  int size[3];                ///< Cell size(Actual volume size)
  int id;                     ///< Block ID
  int level;                  ///< LoD level(0 = finest, 1, 2, ... = coarser)
  const unsigned char *data;  ///< Opaque pointer to linearlized volume data(size[0] * size[1] * size[2] * sizeof(format) * components)
} HIVEVolumeBlock;

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
```

Corresponding data loader is implemented in `lib/Loader/RawSparseVolumeLoader`.

## Usage

Here is the pseudo code of how to use InSitu API in C/C++ application.

 and SceneScript

    // ==== C/C++ side. =====
    SceneScript script;
    
    HIVESparseVolume *sparseVolume = new HIVESparseVolume;
    
    // fill sparseVolume ...
    
    // Create a memory object and set its content. 
    script.CreateMemoryData("SparseVolumeScalar");
    script.SetMemoryData("SparseVolumeScalar", sparseVolume, sizeof(HIVESparseVolume));
    
    // Execute script file
    script.ExecuteFile("myscript.lua", args);
    
    // delete volume data(if required)
    
    // delete data struct.
    delete sparseVolume;

And SceneScript.

    -- ==== Lua side. ====
    
    -- Lookup memory object.
    local name = 'SparseVolumeScalar'
    local data = getMemoryData(name)

    -- Load SparseVolume data from memory object.
    local sparseVol = require("RawSparseVolumeLoader")()
    sparseVol:LoadFromPointer(data.pointer)
    
    ...


 
## Memory lifetime

The pointer passed to In-Situ data strucure is not free'ed or overwritten by HiveCore/SceneScript. 

And the pointer must be valid until the rendering finishes in HiveCore, i.e. the pointer must be valid just before calling `render()` method in SceneScript. 

Users manually frees memory or overwrite data(e.g. the result of next timestep) after the rendering finishes if required.
 
## Example

[HiveCore/test/](HiveCore/test)

## Extending In-Situ visualization

* Define data structure both used for an user application and HiveCore in `include/InSitu`
* Implement custom data loader in `lib/Loader`
* Write lua binding of custom data loader.
