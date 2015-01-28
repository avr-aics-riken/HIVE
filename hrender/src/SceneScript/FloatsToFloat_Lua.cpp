#include "FloatsToFloat_Lua.h"

FloatsToFloat_Lua::FloatsToFloat_Lua(){}

int FloatsToFloat_Lua::Create(BufferVolumeData_Lua* volume, int offset) {
    return FloatsToFloat::Create(volume, offset);
}

int FloatsToFloat_Lua::Component() {
    return FloatsToFloat::Component();
}

BufferVolumeData_Lua* FloatsToFloat_Lua::VolumeData() {
    return new BufferVolumeData_Lua(FloatsToFloat::VolumeData());
}

