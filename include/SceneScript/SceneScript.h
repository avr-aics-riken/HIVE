/**
 * @file SceneScript.h
 * SceneScript Luaラッパー
 */
#ifndef _SCENESCRIPT_H_
#define _SCENESCRIPT_H_
/*
    SceneScript.h
*/
#include <string>
#include <vector>

class UserBufferData;

class SceneScript
{
public:
    SceneScript();
    ~SceneScript();
    
    // for batch execution
    bool Execute(const char* luascript, const std::vector<std::string>& sceneargs);
    bool ExecuteFile(const char* scenefile, const std::vector<std::string>& sceneargs);
  
    // for interactive execution
    void Begin(const std::vector<std::string>& sceneargs);
    bool Command(const char* scenecommand);
    void End();
    
    // in-memory-data binding
    bool CreateMemoryData(const char* dataId);
    bool IsMemoryData(const char* dataId);
    bool DeleteMemoryData(const char* dataId);
    bool DeleteAllMemoryData();
    bool SetMemoryData(const char* dataId, const void* memptr, const size_t memsize);
    size_t GetMemoryDataSize(const char* dataId) const;
    const void* GetMemoryDataPointer(const char* dataId) const;
    int GetMemoryDataNum() const;
    const char* GetMemoryDataId(int i) const;
    
    UserBufferData* GetUserBufferData();
    
    class Impl; // to be public (for internal)
private:
    Impl* m_imp;
};

#endif //_SCENESCRIPT_H_
