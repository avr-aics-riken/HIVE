/*
    Loader.h
*/
#ifndef _LOADER_H_
#define _LOADER_H_


#include "LuaUtil.h"
#include "Buffer.h"


//------//------//------//------//------//------//------//------//------
//
//------//------//------//------//------//------//------//------//------
class OBJLoader : public RefCount
{
public:
	OBJLoader();
	~OBJLoader();
	
	void Clear();
	bool Load(const char* filename);
	Vec3Buffer* Position() { return m_pos;    }
    Vec3Buffer* Normal()   { return m_normal; }
    Vec2Buffer* Texcoord() { return m_texcoord; }
	UintBuffer* Index()    { return m_index;  }
	FloatBuffer* Material(){ return m_mat;    }
	
private:
	LuaRefPtr<Vec3Buffer>  m_pos;
    LuaRefPtr<Vec3Buffer>  m_normal;
    LuaRefPtr<Vec2Buffer>  m_texcoord;
	LuaRefPtr<UintBuffer>  m_index;
	LuaRefPtr<FloatBuffer> m_mat;
	
public:
	LUA_SCRIPTCLASS_BEGIN(OBJLoader)
	LUA_SCRIPTCLASS_METHOD_ARG0(Vec3Buffer*,Position)
	LUA_SCRIPTCLASS_METHOD_ARG0(Vec3Buffer*,Normal)
   	LUA_SCRIPTCLASS_METHOD_ARG0(Vec2Buffer*,Texcoord)
	LUA_SCRIPTCLASS_METHOD_ARG0(UintBuffer*,Index)
	LUA_SCRIPTCLASS_METHOD_ARG0(FloatBuffer*,Material)
	LUA_SCRIPTCLASS_METHOD_ARG1(bool,Load,const char*)
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(OBJLoader);

#endif //_LOADER_H_

