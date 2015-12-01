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
    bool Execute(const char* scenefile, const std::vector<std::string>& sceneargs);
    
};

#endif //_SCENESCRIPT_H_
