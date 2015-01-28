#include "VolLoader_Lua.h"

VOLLoader_Lua::VOLLoader_Lua()
{
}

VOLLoader_Lua::~VOLLoader_Lua()
{
}

void VOLLoader_Lua::Clear()
{
    VOLLoader::Clear();
}

bool VOLLoader_Lua::Load(const char* filename)
{
    return VOLLoader::Load(filename);
}

int VOLLoader_Lua::Width() {
    return VOLLoader::Width();
}

int VOLLoader_Lua::Height() {
    return VOLLoader::Height();
}

int VOLLoader_Lua::Depth() {
    return VOLLoader::Depth();
}

int VOLLoader_Lua::Component() {
    return VOLLoader::Component();
}

