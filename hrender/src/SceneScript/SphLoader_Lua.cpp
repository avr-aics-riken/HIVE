#include "SphLoader_Lua.h"

SPHLoader_Lua::SPHLoader_Lua()
{
}

SPHLoader_Lua::~SPHLoader_Lua()
{
}

int SPHLoader_Lua::Width() {
    return Width();
}

int SPHLoader_Lua::Height() {
    return Height();
}

int SPHLoader_Lua::Depth() {
    return Depth();
}

int SPHLoader_Lua::Component() {
    return Component();
}


bool SPHLoader_Lua::Load(const char* filename)
{
    return SPHLoader::Load(filename);
}

BufferVolumeData_Lua* SPHLoader_Lua::VolumeData() {
    return new BufferVolumeData_Lua(SPHLoader::VolumeData());
}

