/*
 *  LuaUtil.h
 * 
 */

#ifndef INCLUDE_MOE_LUAUTIL_H
#define INCLUDE_MOE_LUAUTIL_H

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include <assert.h>
#include <map>
#include <string>

template<typename T>
inline T luaX_Cast(lua_State* L, int i) {
    return lua_tonumber(L, i);
}
template<>
inline std::string luaX_Cast<std::string>(lua_State* L, int i)
{
	const char* s = lua_tostring(L, i);
	if (!s)
		return std::string();
    return std::string(s);
}

template<typename T>
inline bool isLuaType(lua_State* L,int idx) {
	assert(0); // Unkown type
	return false;
}
template<> inline bool isLuaType<int>(lua_State* L, int idx)        { return lua_isnumber(L, idx); }
template<> inline bool isLuaType<std::string>(lua_State* L, int idx){ return lua_isstring(L, idx); }
template<> inline bool isLuaType<const std::string&>(lua_State* L, int idx){ return lua_isstring(L, idx); }
template<> inline bool isLuaType<float>(lua_State* L, int idx)      { return lua_isnumber(L, idx); }
template<> inline bool isLuaType<double>(lua_State* L, int idx)     { return lua_isnumber(L, idx); }
template<> inline bool isLuaType<void*>(lua_State* L, int idx)      { return lua_isuserdata(L, idx); }

template<typename T>
inline T eval(lua_State* L, const char* str, ...)
{
    char buf[128] = {};
    
    va_list args;
    va_start(args, str);
    vsprintf(buf, str, args);
    va_end(args);
    
    luaL_dostring(L, buf);
    T val = luaX_Cast<T>(L, -1);
    lua_pop(L, 1);
    return val;
}

template<typename T>
inline bool eval(lua_State* L, T& val, const char* str, ...)
{
    char buf[128] = {};
    
    va_list args;
    va_start(args, str);
    vsprintf(buf, str, args);
    va_end(args);
    
    luaL_dostring(L, buf);
	if (isLuaType<T>(L,-1)) {
		val = luaX_Cast<T>(L, -1);
		lua_pop(L, 1);
		return true;
	}
    lua_pop(L, 1);
    return false;
}

inline void dumpStack(lua_State* L)
{
    const int stackSize = lua_gettop(L);
    for(int i = stackSize; i >= 1; i-- ) {
        int type = lua_type(L, i);
        printf("Stack[%2d-%10s] : ", i, lua_typename(L,type) );
        switch( type ) {
            case LUA_TNUMBER:  printf("%f", lua_tonumber(L, i) ); break;
            case LUA_TBOOLEAN: printf("%s", lua_toboolean(L, i) ? "true" : "false"); break;
            case LUA_TSTRING:  printf("%s", lua_tostring(L, i) ); break;
            case LUA_TNIL:     break;
			case LUA_TUSERDATA:printf("%s:%p", lua_typename(L, type), *reinterpret_cast<void**>(lua_touserdata(L,i))); break;
            default:           printf("%s", lua_typename(L, type)); break;
        }
        printf("\n");
    }
    printf("\n");
}
inline int getTableNum(lua_State* L, const char* tablename)
{
#if 1
	std::string buf = std::string("local n = 0; for i,v in pairs(") + std::string(tablename) + std::string(") do n=n+1 end return n;");
	luaL_dostring(L, buf.c_str());
	int n = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return n;
#else
	lua_getglobal(L, tablename);
	if (!lua_istable(L, -1))
		return 0;
	lua_pushnil(L);
	int n = 0;
	while(lua_next(L, -2) != 0){
		++n;
		lua_pop(L, 1);
	}
	return n;
#endif
}

inline int getTableValues(lua_State* L, const char* tablename, std::map<std::string,std::string>& vals)
{
	lua_getglobal(L, tablename);
	lua_pushnil(L);
	while(lua_next(L, -2) != 0){
		if(lua_isstring(L, -1)){
			std::string fld = static_cast<std::string>(lua_tostring(L, -2));
			std::string str = static_cast<std::string>(lua_tostring(L, -1));
			vals[std::string(fld)] = str;
		}
		lua_pop(L, 1);
	}
	return static_cast<int>(vals.size());
}

inline void SetFunction(lua_State* L, const char* functionname, int (*func)(lua_State*))
{
	lua_register(L, functionname, func);
}


inline lua_State* createLua()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    return L;
}

inline bool doLua(lua_State* L, const char* lua)
{
    int r = luaL_dostring(L, lua);
    if (r){
        const char *err = lua_tostring(L, -1);
        printf("ERROR: %s\n", err);
        return false;
    }
    return true;
}

inline void closeLua(lua_State* L)
{
    lua_close(L);
}

// ---------- Lua Class wrapper --------------
#ifdef SCRIPT_DEBUG
#define _STRACE printf
#else
#define _STRACE(...)
#endif

// --- Reference count ---

class LuaRef
{
protected:
	LuaRef()         { m_ref = 0; }
	virtual ~LuaRef(){};
	lua_State* L;
public:
	int Ref()          { return ++m_ref; }
	int GetRef() const { return m_ref; }
	int Unref()        {
		const int ref = --m_ref;
		assert(ref>=0);
		if (!ref)
			delete this;
		return ref;
	}
	void SetL(lua_State* ls) { L = ls; }
private:
	int m_ref;
};

template<class T>
class LuaRefPtr
{
public:
	LuaRefPtr()
	{
		m_p = 0;
	}
	LuaRefPtr(T* s)
	{
		m_p = s;
		if (m_p)
			m_p->Ref();
	}
	LuaRefPtr(const LuaRefPtr& p)
	{
		m_p = p;
		if (m_p)
			m_p->Ref();
	}
	~LuaRefPtr()
	{
		if (m_p)
			m_p->Unref();
	}
	
	T* Get() const
	{
		return m_p;
	}
	
	operator T*() const
	{
		return m_p;
	}
	T& operator *() const
	{
		assert(m_p);
		return *m_p;
	}
	T* operator->() const
	{
		assert(m_p);
		return m_p;
	}
	bool operator !() const
	{
		return m_p == 0;
	}
	bool operator ==(T* p) const
	{
		return m_p == p;
	}
	bool operator !=(T* p) const
	{
		return m_p != p;
	}
	bool operator <(T* p) const // for STL container
	{
		return m_p < p;
	}
	
	//equal operator
	T* operator =(T* p)
	{
		if (p)
			p->Ref();
		if (m_p)
			m_p->Unref();
		m_p = p;
		return m_p;
	}
	const LuaRefPtr& operator =(const LuaRefPtr& r)
	{
		if (this->m_p == r.m_p)
			return *this;
		
		if (r.m_p)
			r.m_p->Ref();
		if (m_p)
			m_p->Unref();
		m_p = r.m_p;
		
		return *this;
	}
	
private:
	T* m_p;
};


// --- LUACAST / LUAPUSH template ---

template <class T1>
inline T1 LUACAST(lua_State* L, int a1) {
    _STRACE("Error unknown cast type.");
    assert(0);
	return T1();
}
template <class T>
inline int LUAPUSH(lua_State* L, T r) {
    _STRACE("Error unknown push type.\n");
    assert(0);
	return 0;
}
// --- LUACAST specialization ---

template <> inline int LUACAST<int>(lua_State* L, int argi) {
    return static_cast<int>(lua_tointeger(L, argi));
}
template <> inline unsigned int LUACAST<unsigned int>(lua_State* L, int argi) {
    return static_cast<unsigned int>(lua_tointeger(L, argi));
}
template <> inline float LUACAST<float>(lua_State* L, int argi) {
    return static_cast<float>(lua_tonumber(L, argi));
}
template <> inline double LUACAST<double>(lua_State* L, int argi) {
    return lua_tonumber(L, argi);
}
template <> inline std::string LUACAST<std::string>(lua_State* L, int argi) {
    return std::string(lua_tostring(L, argi));
}
template <> inline const std::string& LUACAST<const std::string&>(lua_State* L, int argi) {
	static std::string tmpstr;
	tmpstr = std::string(lua_tostring(L, argi));
    return tmpstr;
}
template <> inline void* LUACAST<void*>(lua_State* L, int argi) {
    return lua_touserdata(L, argi);
}
template <> inline const char* LUACAST<const char*>(lua_State* L, int argi) {
    return lua_tostring(L, argi);
}
template <> inline bool LUACAST<bool>(lua_State* L, int argi) {
    return lua_toboolean(L, argi) != 0;
}

// --- LUAPUSH specialization ---
template <> inline int LUAPUSH<bool>(lua_State* L, bool val) {
    lua_pushboolean(L, val ? 1 : 0);
	return 1;
}
template <> inline int LUAPUSH<int>(lua_State* L, int val) {
    lua_pushinteger(L, val);
	return 1;
}
template <> inline int LUAPUSH<unsigned int>(lua_State* L, unsigned int val) {
    lua_pushunsigned(L, val);
	return 1;
}
template <> inline int LUAPUSH<double>(lua_State* L, double val) {
    lua_pushnumber(L, val);
	return 1;
}
template <> inline int LUAPUSH<float>(lua_State* L, float val) {
    lua_pushnumber(L, val);
	return 1;
}
template <> inline int LUAPUSH<std::string>(lua_State* L, std::string val) {
    lua_pushstring(L, val.c_str());
	return 1;
}

#define LUA_SCRIPTCLASS_CAST_AND_PUSH(CLASSNAME) \
	template <> inline CLASSNAME* LUACAST<CLASSNAME*>(lua_State* L, int argi) { \
		_STRACE("LUACAST " #CLASSNAME );\
		/*return *static_cast<CLASSNAME**>(lua_touserdata(L, argi));*/ \
		if (lua_isuserdata(L, argi)) { \
			CLASSNAME* inst = **static_cast<LuaRefPtr<CLASSNAME>**>(lua_touserdata(L, argi)); \
			_STRACE(" : %p\n", inst);\
			return inst; \
		} else { \
			return 0; \
		}\
	} \
	template <> inline int LUAPUSH<CLASSNAME*>(lua_State* L, CLASSNAME* val) { \
		_STRACE("LUAPUSH " #CLASSNAME " : %p\n", val);\
		if (val) { \
/*			CLASSNAME** object = (CLASSNAME**)lua_newuserdata(L,sizeof(CLASSNAME*));*/ \
/*			*object = val;*/ \
			LuaRefPtr<CLASSNAME>** object = (LuaRefPtr<CLASSNAME>**)lua_newuserdata(L,sizeof(LuaRefPtr<CLASSNAME>*)); \
			*object = new LuaRefPtr<CLASSNAME>(val); \
			luaL_getmetatable(L, #CLASSNAME "_metatable"); \
			lua_setmetatable(L, -2); \
		} else { \
			lua_pushnil(L); \
		} \
		return 1; \
	}


#define LUA_SCRIPTCLASS_BEGIN(CLASSNAME) \
	static int LUA_CLASS_DESTRUCTOR(lua_State* L) { \
		_STRACE("Called DESTRUCTOR: " #CLASSNAME "\n"); \
		/*CLASSNAME* object=*(CLASSNAME**)lua_touserdata(L, 1);*/ \
		/*delete object;*/ \
		LuaRefPtr<CLASSNAME>* rptr = *static_cast<LuaRefPtr<CLASSNAME>**>(lua_touserdata(L, 1)); \
		_STRACE("  RefCount = %d\n", (*rptr)->GetRef()); \
		delete rptr; \
		return 0;\
	} \
	static void LUA_CLASS_REGISTER_DESTRUCTOR(lua_State* L) { \
		lua_pushcfunction(L, &CLASSNAME::LUA_CLASS_DESTRUCTOR); \
		lua_setfield(L, -2, "__gc"); \
	} \
	static int LUA_CLASS_NEW(lua_State* L) { \
		typedef CLASSNAME ICLASS; \
		/*ICLASS** object=(ICLASS**)lua_newuserdata(L,sizeof(CLASSNAME*));*/ \
		/* *object = new CLASSNAME();*/ \
		LuaRefPtr<CLASSNAME>** object = (LuaRefPtr<CLASSNAME>**)lua_newuserdata(L,sizeof(LuaRefPtr<CLASSNAME>*)); \
		*object = new LuaRefPtr<CLASSNAME>(new CLASSNAME()); \
		(**object)->SetL(L); \
		luaL_getmetatable(L, #CLASSNAME "_metatable");  \
		lua_setmetatable(L, -2); \
		return 1; \
	} \
	static int LUA_CLASS_CREATEMETATABLE(lua_State* L) { \
		typedef CLASSNAME ICLASS; \
		luaL_newmetatable(L, #CLASSNAME "_metatable"); \
		lua_newtable(L); \
		_STRACE("[REGISTER] %s class register -> %s\n", #CLASSNAME, #CLASSNAME "_metatable");

#define LUA_SCRIPTCLASS_END() \
		lua_setfield(L,-2,"__index"); \
		LUA_CLASS_REGISTER_DESTRUCTOR(L); \
		return 1; \
	}\

#define INTERNAL_LUA_SCRIPTCLASS_METHOD_BEGIN(RET,FUNCNAME) \
		_STRACE("[REGISTER] %s ClassFunc::%s\n", #RET, #FUNCNAME); \
		class luafuncClass##FUNCNAME{ \
		public: \
			static int func(lua_State* L) { \
				_STRACE("Called LuaFunc %s LUAFUNC_%s\n", #RET, #FUNCNAME); \
				/* ICLASS* thisptr = *(ICLASS**)lua_touserdata(L,1);*/ \
				ICLASS* thisptr = **static_cast<LuaRefPtr<ICLASS>**>(lua_touserdata(L, 1));


#define INTERNAL_LUA_SCRIPTCLASS_METHOD_END(RET,FUNCNAME) \
			} \
		}; \
		lua_pushcfunction(L,luafuncClass##FUNCNAME::func); \
		lua_setfield(L,-2,#FUNCNAME);

#define LUA_SCRIPTCLASS_METHOD_ARG0(RET,FUNCNAME) \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_BEGIN(RET,FUNCNAME) \
		return LUAPUSH<RET>(L,thisptr->FUNCNAME()); \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_END(RET,FUNCNAME)

#define LUA_SCRIPTCLASS_METHOD_ARG1(RET,FUNCNAME,ARG1) \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_BEGIN(RET,FUNCNAME) \
		ARG1 arg1 = LUACAST<ARG1>(L, 2); \
		return LUAPUSH<RET>(L,thisptr->FUNCNAME(arg1)); \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_END(RET,FUNCNAME)

#define LUA_SCRIPTCLASS_METHOD_ARG2(RET,FUNCNAME,ARG1,ARG2) \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_BEGIN(RET,FUNCNAME) \
		ARG1 arg1 = LUACAST<ARG1>(L, 2); \
		ARG2 arg2 = LUACAST<ARG2>(L, 3); \
		return LUAPUSH<RET>(L,thisptr->FUNCNAME(arg1,arg2)); \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_END(RET,FUNCNAME)

#define LUA_SCRIPTCLASS_METHOD_ARG3(RET,FUNCNAME,ARG1,ARG2,ARG3) \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_BEGIN(RET,FUNCNAME) \
		ARG1 arg1 = LUACAST<ARG1>(L, 2); \
		ARG2 arg2 = LUACAST<ARG2>(L, 3); \
		ARG3 arg3 = LUACAST<ARG3>(L, 4); \
		return LUAPUSH<RET>(L,thisptr->FUNCNAME(arg1,arg2,arg3)); \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_END(RET,FUNCNAME)

#define LUA_SCRIPTCLASS_METHOD_ARG4(RET,FUNCNAME,ARG1,ARG2,ARG3,ARG4) \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_BEGIN(RET,FUNCNAME) \
		ARG1 arg1 = LUACAST<ARG1>(L, 2); \
		ARG2 arg2 = LUACAST<ARG2>(L, 3); \
		ARG3 arg3 = LUACAST<ARG3>(L, 4); \
		ARG4 arg4 = LUACAST<ARG4>(L, 5); \
		return LUAPUSH<RET>(L,thisptr->FUNCNAME(arg1,arg2,arg3,arg4)); \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_END(RET,FUNCNAME)

#define LUA_SCRIPTCLASS_METHOD_ARG5(RET,FUNCNAME,ARG1,ARG2,ARG3,ARG4,ARG5) \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_BEGIN(RET,FUNCNAME) \
		ARG1 arg1 = LUACAST<ARG1>(L, 2); \
		ARG2 arg2 = LUACAST<ARG2>(L, 3); \
		ARG3 arg3 = LUACAST<ARG3>(L, 4); \
		ARG4 arg4 = LUACAST<ARG4>(L, 5); \
		ARG5 arg5 = LUACAST<ARG5>(L, 6); \
		return LUAPUSH<RET>(L,thisptr->FUNCNAME(arg1,arg2,arg3,arg4,arg5)); \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_END(RET,FUNCNAME)


#define LUA_SCRIPTCLASS_NEW_FUNCTION(CLASSNAME) CLASSNAME::LUA_CLASS_NEW
#define LUA_SCRIPTCLASS_REGISTER(L,CLASSNAME) CLASSNAME::LUA_CLASS_CREATEMETATABLE(L)



#endif // INCLUDE_MOE_LUAUTIL_H
