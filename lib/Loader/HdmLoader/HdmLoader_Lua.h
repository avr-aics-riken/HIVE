/**
 * @file HDMLoader_Lua.h
 * HDMLoader Luaラッパー
 */
#ifndef _HDMLOADER_LUA_H_
#define _HDMLOADER_LUA_H_

#include "LuaUtil.h"
#include "HdmLoader.h"
#include "BufferSparseVolumeData_Lua.h"
/**
 * HDMLoader Luaラッパー
 */
class HDMLoader_Lua : public HDMLoader
{
public:
    HDMLoader_Lua() {}
    ~HDMLoader_Lua() {}

    int Width() {
        return Width();
    }

    int Height() {
        return Height();
    }

    int Depth() {
        return Depth();
    }

    int Component() {
        return Component();
    }

    bool Init(const char* cellidFilename, const char* dataFilename) {
        return HDMLoader::Init(cellidFilename, dataFilename);
    }

    BufferSparseVolumeData_Lua* LoadField(const char* fieldName, const char* fieldType, int components, int timeStepIndex = 0, int virtualCells=2) {
		RefPtr<BufferSparseVolumeData> p = HDMLoader::LoadField(fieldName, fieldType, components, timeStepIndex, virtualCells);

		if (p) {
            return BufferSparseVolumeData_Lua::CreateInstance(p);
		} else {
			return NULL;
		}
    }

    LUA_SCRIPTCLASS_BEGIN(HDMLoader_Lua)
    LUA_SCRIPTCLASS_METHOD_ARG2(bool,Init,const char*, const char*)
    LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG5("HDMLoader", BufferSparseVolumeData_Lua*,LoadField, const char*, const char*, int, int, int)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(HDMLoader_Lua);

#endif //_HDMLOADER_LUA_H_

