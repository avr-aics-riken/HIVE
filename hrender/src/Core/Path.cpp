/**
 * @file Path.cpp
 * プラットフォーム別ファイルパス操作ユーティリティ
 */

#include <string>
#include <stdio.h>
#include "Path.h"

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#undef __BLOCKS__ // avoid BCMLib::block.h miss include
#include <CoreFoundation/CFBundle.h>
#else
#include <unistd.h>
#endif

/**
 * 実行している自身のファイルパス取得
 * @retval 実行ファイルフルパス
 */
std::string getBinaryDir()
{
    const int MAXPATHLEN = 4096;
    char exepath[MAXPATHLEN] = {};
#if _WIN32
    assert(0);// TODO
    return std::string(exepath);
    
#elif __APPLE__
    CFBundleRef bundle         = CFBundleGetMainBundle();
    CFURLRef    executableURL  = CFBundleCopyExecutableURL(bundle);
    CFStringRef executablePath = CFURLCopyFileSystemPath(executableURL, kCFURLPOSIXPathStyle);
    CFStringGetMaximumSizeOfFileSystemRepresentation(executablePath);
    CFStringGetFileSystemRepresentation(executablePath, exepath, MAXPATHLEN);
    CFRelease(executablePath);
    CFRelease(executableURL);
#else // Linux
    readlink("/proc/self/exe", exepath, sizeof(exepath));
#endif
    // for Mac & Linux
    std::string fullpath(exepath);
    size_t t = fullpath.rfind("/");
    if (t != std::string::npos) {
        fullpath = fullpath.substr(0, t + 1);
    }
    return fullpath;
}


#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define chdir _chdir
#else
#include <unistd.h>
#endif
#include <string>

/**
 * カレントディレクトリパス取得
 * @retval カレントディレクトリパス
 */
std::string getCurrentDir()
{
    char buffer[1024] = {0};
    char *temp = NULL;
    std::string ret = "";
#if _WIN32
    DWORD dwRet = GetCurrentDirectory(sizeof(buffer), (LPTSTR)buffer);
    if(!dwRet)
    {
        fprintf(stderr,"Failed get current directory.");
        return ret;
    }
#elif __APPLE__
    temp = getcwd(buffer, sizeof(buffer));
    if(!temp) 
    {
        fprintf(stderr,"Failed get current directory.");
        return ret;
    }
#else // Linux
    temp = getcwd(buffer, sizeof(buffer));
    if(!temp) 
    {
        fprintf(stderr,"Failed get current directory.");
        return ret;
    }
#endif
    ret = buffer;
    ret = ret + "/";
    return ret;
}

/**
 * カレントディレクトリパス変更
 * @param  filefullpath 変更先パス
 */
void changeFileDir(const std::string& filefullpath)
{
#if _WIN32
    assert(0); // TODO
#else
    std::string scenepath = filefullpath;
    const size_t p = scenepath.rfind("/");
    if (p != std::string::npos) {
        scenepath = scenepath.substr(0, p + 1);
        chdir(scenepath.c_str());
    }
#endif
}

/**
 * 相対パスからフルパス変換
 * @param  path 相対パス
 * @retval フルパス
 */
std::string convertFullpath(const std::string& path)
{
#if _WIN32
    assert(0); // TODO    
#else // Mac & Linux
    if (path.find("/") == 0) {
        return path;
    } else {
        //std::string exedir = getBinaryDir();
        //std::string fullpath = exedir + path;
        std::string currDir = getCurrentDir();
        std::string fullpath = currDir + path;
        return fullpath;
    }
#endif
}
