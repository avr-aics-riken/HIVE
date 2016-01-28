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

class SceneScript
{
public:
    SceneScript();
    ~SceneScript();
    bool Execute(const char* luascript, const std::vector<std::string>& sceneargs);
    bool ExecuteFile(const char* scenefile, const std::vector<std::string>& sceneargs);
  
    void Begin(const std::vector<std::string>& sceneargs);
    bool Command(const char* scenecommand);
    void End();
private:
    class Impl;
    Impl* m_imp;
};

#endif //_SCENESCRIPT_H_
