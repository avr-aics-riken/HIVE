/**
 * @file LuaUtil.h
 * Luaユーティリティ
 */

#ifndef INCLUDE_MOE_LUAUTIL_H
#define INCLUDE_MOE_LUAUTIL_H

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include <assert.h>
#include <vector>
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
        fprintf(stderr, "ERROR: %s\n", err);
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

// --- Reference count ----
#include "../Core/Ref.h"

#define LuaRef Ref
#define LuaRefPtr RefPtr


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
template <> inline void* const LUACAST<void* const>(lua_State* L, int argi) {
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
template <> inline int LUAPUSH<const char*>(lua_State* L, const char* val) {
    lua_pushlightuserdata(L, (void*)val);
    return 1;
}
template <> inline int LUAPUSH<void*>(lua_State* L, void* val) {
    lua_pushlightuserdata(L, val);
    return 1;
}
template <> inline int LUAPUSH<void* const>(lua_State* L, void* const val) {
    lua_pushlightuserdata(L, val);
    return 1;
}
template <> inline int LUAPUSH<std::string>(lua_State* L, std::string val) {
    lua_pushlstring(L, val.c_str(), val.size());
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
		(**object)->SetUserData(L); \
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
        lua_pop(L, 1); \
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

#define LUA_SCRIPTCLASS_METHOD_ARG6(RET,FUNCNAME,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6) \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_BEGIN(RET,FUNCNAME) \
		ARG1 arg1 = LUACAST<ARG1>(L, 2); \
		ARG2 arg2 = LUACAST<ARG2>(L, 3); \
		ARG3 arg3 = LUACAST<ARG3>(L, 4); \
		ARG4 arg4 = LUACAST<ARG4>(L, 5); \
		ARG5 arg5 = LUACAST<ARG5>(L, 6); \
		ARG6 arg6 = LUACAST<ARG6>(L, 7); \
		return LUAPUSH<RET>(L,thisptr->FUNCNAME(arg1,arg2,arg3,arg4,arg5,arg6)); \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_END(RET,FUNCNAME)

#define LUA_SCRIPTCLASS_METHOD_ARG7(RET,FUNCNAME,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7) \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_BEGIN(RET,FUNCNAME) \
		ARG1 arg1 = LUACAST<ARG1>(L, 2); \
		ARG2 arg2 = LUACAST<ARG2>(L, 3); \
		ARG3 arg3 = LUACAST<ARG3>(L, 4); \
		ARG4 arg4 = LUACAST<ARG4>(L, 5); \
		ARG5 arg5 = LUACAST<ARG5>(L, 6); \
		ARG6 arg6 = LUACAST<ARG6>(L, 7); \
		ARG7 arg7 = LUACAST<ARG7>(L, 8); \
		return LUAPUSH<RET>(L,thisptr->FUNCNAME(arg1,arg2,arg3,arg4,arg5,arg6,arg7)); \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_END(RET,FUNCNAME)

#define LUA_SCRIPTCLASS_METHOD_ARG8(RET,FUNCNAME,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8) \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_BEGIN(RET,FUNCNAME) \
		ARG1 arg1 = LUACAST<ARG1>(L, 2); \
		ARG2 arg2 = LUACAST<ARG2>(L, 3); \
		ARG3 arg3 = LUACAST<ARG3>(L, 4); \
		ARG4 arg4 = LUACAST<ARG4>(L, 5); \
		ARG5 arg5 = LUACAST<ARG5>(L, 6); \
		ARG6 arg6 = LUACAST<ARG6>(L, 7); \
		ARG7 arg7 = LUACAST<ARG7>(L, 8); \
		ARG7 arg8 = LUACAST<ARG8>(L, 9); \
		return LUAPUSH<RET>(L,thisptr->FUNCNAME(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8)); \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_END(RET,FUNCNAME)

#define LUA_SCRIPTCLASS_METHOD_ARG10(RET,FUNCNAME,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10) \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_BEGIN(RET,FUNCNAME) \
		ARG1 arg1  = LUACAST<ARG1>(L, 2); \
		ARG2 arg2  = LUACAST<ARG2>(L, 3); \
		ARG3 arg3  = LUACAST<ARG3>(L, 4); \
		ARG4 arg4  = LUACAST<ARG4>(L, 5); \
		ARG5 arg5  = LUACAST<ARG5>(L, 6); \
		ARG5 arg6  = LUACAST<ARG6>(L, 7); \
		ARG5 arg7  = LUACAST<ARG7>(L, 8); \
		ARG5 arg8  = LUACAST<ARG8>(L, 9); \
		ARG5 arg9  = LUACAST<ARG9>(L,10); \
		ARG5 arg10 = LUACAST<ARG10>(L,11); \
		return LUAPUSH<RET>(L,thisptr->FUNCNAME(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)); \
	INTERNAL_LUA_SCRIPTCLASS_METHOD_END(RET,FUNCNAME)

#define LUA_SCRIPTCLASS_NEW_FUNCTION(CLASSNAME) CLASSNAME::LUA_CLASS_NEW
#define LUA_SCRIPTCLASS_REGISTER(L,CLASSNAME) CLASSNAME::LUA_CLASS_CREATEMETATABLE(L)

//-------------------------
// LuaTable class
class LuaTable
{
public:
    enum VALUE_TYPE {
        TYPE_INVALID,
        TYPE_NUMBER,
        TYPE_USERDATA,
        TYPE_STRING,
        TYPE_ARRAY,
        TYPE_MAP,
    };
private:
    std::vector<LuaTable> m_table;
    std::map<std::string, LuaTable> m_map;
    VALUE_TYPE m_type;
    double m_number;
    std::string m_string;
    void* m_userdata;
    
public:
    LuaTable() : m_type(TYPE_ARRAY), m_number(0.0), m_userdata(0) {}
    VALUE_TYPE GetType() const { return m_type; }

    //---------------------------------
    // for initialization
    LuaTable(double num) : m_type(TYPE_NUMBER), m_number(num), m_userdata(0) {}
    LuaTable(const std::string& str) : m_type(TYPE_STRING), m_number(0.0), m_string(str), m_userdata(0) {}
    LuaTable(void* ptr) : m_type(TYPE_USERDATA), m_number(0.0), m_userdata(ptr) {}
    LuaTable(double x, double y, double z, double w) : m_type(TYPE_ARRAY), m_number(0.0), m_userdata(0) {
        m_table.push_back(x);
        m_table.push_back(y);
        m_table.push_back(z);
        m_table.push_back(w);
    }
    LuaTable(double x, double y, double z) : m_type(TYPE_ARRAY), m_number(0.0), m_userdata(0) {
        m_table.push_back(x);
        m_table.push_back(y);
        m_table.push_back(z);
    }
    LuaTable(double x, double y) : m_type(TYPE_ARRAY), m_number(0.0), m_userdata(0) {
        m_table.push_back(x);
        m_table.push_back(y);
    }
    
    
    // from Lua stack
    LuaTable(lua_State* L, int stacki) {
        if (lua_isuserdata(L, stacki)) {
            m_type = TYPE_USERDATA;
            m_userdata = lua_touserdata(L, stacki);
        } else if (lua_isnumber(L, stacki)) {
            m_type = TYPE_NUMBER;
            m_number = lua_tonumber(L, stacki);
        } else if (lua_isstring(L, stacki)) {
            m_type = TYPE_STRING;
            m_string = std::string(lua_tostring(L, stacki));
        } else if (lua_istable(L, stacki)) {
            lua_pushnil(L); // first nil
            while (lua_next(L, stacki) != 0) {
                if (lua_isnumber(L, -2)) {
                    LuaTable val(L, -1);
                    if (val.GetType() != TYPE_INVALID) {
                        if (lua_isnumber(L, -2)) {
                            m_type = TYPE_ARRAY;
                            this->push(val);
                        } else if (lua_isstring(L, -2)) {
                            m_type = TYPE_MAP;
                            const char* s = lua_tostring(L, -2);
                            this->map(s, val);
                        }
                    }
                } else if (lua_istable(L, -2)) {
                    const int top = lua_gettop(L);
                    LuaTable val(L, top);
                    if (lua_isnumber(L, -2)) {
                        m_type = TYPE_ARRAY;
                        this->push(val);
                    } else if (lua_isstring(L, -2)) {
                        m_type = TYPE_MAP;
                        const char* s = lua_tostring(L, -2);
                        this->map(s, val);
                    }
                }
                lua_pop(L, 1);
            }
        } else {
            m_type = TYPE_INVALID;
        }
    }

    //---------------------------------
    // for scalar input operation
    double operator=(double v) {
        m_type = TYPE_NUMBER;
        m_number = v;
        return m_number;
    }
    const std::string& operator=(std::string v) {
        m_type = TYPE_STRING;
        m_string = v;
        return m_string;
    }
    
    //---------------------------------
    // for array operation
    void push(const LuaTable& val)                            { m_type = TYPE_ARRAY; m_table.push_back(val);           }
    void push(double val)                                     { m_type = TYPE_ARRAY; m_table.push_back(LuaTable(val)); }
    void push(const char* val)                                { m_type = TYPE_ARRAY; m_table.push_back(LuaTable(val)); }
    void push(const std::string& val)                         { m_type = TYPE_ARRAY; m_table.push_back(val);           }
    void push(void* ptr)                                      { m_type = TYPE_ARRAY; m_table.push_back(LuaTable(ptr)); }
    //---------------------------------
    // for map operation
    void map(const std::string& name, const LuaTable& val)    { m_type = TYPE_MAP; m_map[name] = val;           }
    void map(const std::string& name, double val)             { m_type = TYPE_MAP; m_map[name] = LuaTable(val); }
    void map(const std::string& name, const char* val)        { m_type = TYPE_MAP; m_map[name] = LuaTable(val); }
    void map(const std::string& name, const std::string& val) { m_type = TYPE_MAP; m_map[name] = LuaTable(val); }
    void map(const std::string& name, void* ptr)              { m_type = TYPE_MAP; m_map[name] = LuaTable(ptr); }
    //---------------------------------
    // Get methods
    double GetNumber() const                                  { return m_number; }
    const std::string& GetString() const                      { return m_string; }
    void* GetUserData() const                                 { return m_userdata; }
    template<typename T> LuaRefPtr<T>* GetUserData() const    {
        if (!m_userdata)
            return 0;
        return *static_cast<LuaRefPtr<T>**>(m_userdata);
    }
    
    const std::vector<LuaTable>& GetTable() const             { return m_table;  }
    const std::map<std::string, LuaTable>& GetMap() const     { return m_map;    }


    //---------------------------------
    // for Lua binding
    //
    void pushLuaTableValue(lua_State* L) const
    {
        const LuaTable::VALUE_TYPE type = GetType();
        if (type == LuaTable::TYPE_ARRAY) {
            lua_newtable(L);
            const std::vector<LuaTable>& table = GetTable();
            for (int i = 0; i < table.size(); ++i) {
                lua_pushnumber(L, i + 1);
                table[i].pushLuaTableValue(L);
                lua_settable(L, -3);
            }
        } else if (type == LuaTable::TYPE_MAP) {
            lua_newtable(L);
            std::map<std::string, LuaTable>::const_iterator it, eit = GetMap().end();
            for (it = GetMap().begin(); it != eit; ++it) {
                lua_pushstring(L, it->first.c_str());
                it->second.pushLuaTableValue(L);
                lua_settable(L, -3);
            }
        } else if (type == LuaTable::TYPE_NUMBER){
            lua_pushnumber(L, GetNumber());
        } else if (type == LuaTable::TYPE_STRING){
            lua_pushstring(L, GetString().c_str());
        } else if (type == LuaTable::TYPE_USERDATA){
            lua_pushlightuserdata(L, GetUserData());
        } else {
            assert(0); // Unknown type
        }
    }
};

template <> inline int LUAPUSH<LuaTable>(lua_State* L, LuaTable val)
{
    val.pushLuaTableValue(L);
    return 1;
}

template <> inline int LUAPUSH<const LuaTable&>(lua_State* L, const LuaTable& val)
{
    val.pushLuaTableValue(L);
    return 1;
}

template <> inline LuaTable LUACAST<LuaTable>(lua_State* L, int argi) {
    return LuaTable(L, argi);
}


//-------------------


#endif // INCLUDE_MOE_LUAUTIL_H
