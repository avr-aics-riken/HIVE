# How to create custom class.

## C/C++ class

Create new class(e.g. MyVolumeToMeshData) by copying one of existing class(e.g. src/Builder/VolumeToMeshData.cpp)

```
// MyVolumeToMeshData.h
#ifndef _MYVOLUMETOMESHDATA_H_
#define _MYVOLUMETOMESHDATA_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>
#include "BufferVolumeData.h"
#include "BufferMeshData.h"

class MyVolumeToMeshData : public RefCount
{
private:
    BufferMeshData*           m_mesh;
    double                    m_isovalue;
    BufferVolumeData*         m_volume;   // mutable
public:
    VolumeToMeshData();
    int Create(BufferVolumeData *volume);
    void SetIsoValue(double isovalue);

    // Build isosurface mesh.
    int IsoSurface();

    BufferMeshData* MeshData();
};

#endif //_MYVOLUMETOMESHDATA_H_
```

And write your own implementation in `MyVolumeToMeshData.cpp`

## Lua wrapper class

Add lua interface wrapper class in `src/SceneScript`

```
// MyVolumeToMeshData_Lua.h

#ifndef _MYVOLUMETOMESHDATA_LUA_H_
#define _MYVOLUMETOMESHDATA_LUA_H_

#include "LuaUtil.h"
#include "MYVolumeToMeshData.h"
#include "BufferVolumeData_Lua.h"
#include "BufferMeshData_Lua.h"

class MyVolumeToMeshData_Lua : public MyVolumeToMeshData
{
public:
    VolumeToMeshData_Lua(){}

    int Create(BufferVolumeData_Lua* volume) {
        return MyVolumeToMeshData::Create(volume);
    }

    bool SetIsoValue(float isovalue) {
        MyVolumeToMeshData::SetIsoValue(static_cast<double>(isovalue));
        return true;
    }

    /// Generate isosurface mesh.
    /// Valid after SetIsoValue()
    int IsoSurface() {
        return MyVolumeToMeshData::IsoSurface();
    }

    /// Retrieve isosurface mesh.
    /// Valid after IsoSurface()
    BufferMeshData_Lua* MeshData() {
        return new BufferMeshData_Lua(MyVolumeToMeshData::MeshData());
    }

    LUA_SCRIPTCLASS_BEGIN(MyVolumeToMeshData_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG1(int,Create,BufferVolumeData_Lua*)
    LUA_SCRIPTCLASS_METHOD_ARG1(int,SetIsoValue, float)
    LUA_SCRIPTCLASS_METHOD_ARG0(int, IsoSurface)
    LUA_SCRIPTCLASS_METHOD_ARG0(BufferMeshData_Lua*,MeshData)
    LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(MyVolumeToMeshData_Lua);

#endif //_MyVOLUMETOMESHDATA_LUA_H_
```

## Register lua wapper to scene script

Register lua wrapper class to `src/SceneScript/SceneScript.cpp`

```
...
#include "MyVolumeToMeshData_Lua.h"

...

void RegisterSceneClass(lua_State* L)
{
  ...
  LUA_SCRIPTCLASS_REGISTER(L, MyVolumeToMeshData_Lua);
  SetFunction(L, "MyVolumeToMeshData",    LUA_SCRIPTCLASS_NEW_FUNCTION(MyVolumeToMeshData_Lua));
  ...
}
```

Add `MyVolumeToMeshData.cpp` and `MyVolumeToMeshData.h` to `hrender/CMakeLists.txt`

Re-run cmake and rebuild `hrender`.

### Calling custom class from a scene file.

Then you can call custom class(`MyVolumeToMeshData`) from a scene file.

```
local surfacer = MyVolumeToMeshData()

local isovalue = 0.0005
surfacer:Create(volumedata)
surfacer:SetIsoValue(isovalue)
surfacer:IsoSurface()

local isosurface = surfacer:MeshData()

...
```
