/**
 * @file Path.cpp
 * プラットフォーム別ファイルパス操作ユーティリティ
 */

#include <string>
#include <stdio.h>
#include "Path.h"

#ifdef _WIN32
#include <windows.h>
#include <assert.h>
#include <tchar.h>
#include <codecvt>
#include <iomanip>
#include <locale>
#include <wchar.h>
#elif __APPLE__
#if __MAC_OS_X_VERSION_MAX_ALLOWED >= 1012
// Carbon is deprecated from macOS Sierra(10.12)
// http://stackoverflow.com/questions/1528298/get-path-of-executable
#include <mach-o/dyld.h>
#else
// Use Carbon
#undef __BLOCKS__ // avoid BCMLib::block.h miss include
#include <CoreFoundation/CFBundle.h>
#endif
#else
#include <unistd.h>
#endif

/**
 * 実行している自身のディレクトリパス取得
 * @return 実行ディレクトリフルパス
 */
std::string getBinaryDir()
{
    const int MAXPATHLEN = 4096;
    char exepath[MAXPATHLEN] = {};
#if _WIN32
	wchar_t app_full_path[1024];
	DWORD length = GetModuleFileNameW(NULL, app_full_path, sizeof(app_full_path) / sizeof(wchar_t));
	std::wstring str(app_full_path, length);
	const char16_t* p = reinterpret_cast<const char16_t*>(str.c_str());
	std::u16string u16str(p);
	// utf16 to utf8
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	std::string stdstr = convert.to_bytes(u16str);

	std::string::size_type pos = stdstr.find_last_of("\\");
	if (pos != std::string::npos) {
		std::string basepath = stdstr.substr(0, pos + 1);
		return basepath;
	}
	return stdstr;
    
#elif __APPLE__
#if __MAC_OS_X_VERSION_MAX_ALLOWED >= 1012
    uint32_t size = sizeof(exepath);
    int ret = _NSGetExecutablePath(exepath, &size);
    if (0 != ret) {
        return ""; // FIXME(IDS): 
    } 
#else
    CFBundleRef bundle         = CFBundleGetMainBundle();
    CFURLRef    executableURL  = CFBundleCopyExecutableURL(bundle);
    CFStringRef executablePath = CFURLCopyFileSystemPath(executableURL, kCFURLPOSIXPathStyle);
    CFStringGetMaximumSizeOfFileSystemRepresentation(executablePath);
    CFStringGetFileSystemRepresentation(executablePath, exepath, MAXPATHLEN);
    CFRelease(executablePath);
    CFRelease(executableURL);
#endif
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
 * @return カレントディレクトリパス
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
	std::string::size_type pos = filefullpath.find_last_of("\\");
	if (pos != std::string::npos) {
		std::string basepath = filefullpath.substr(0, pos);
		SetCurrentDirectory(basepath.c_str());
	}
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
 * @return フルパス
 */
std::string convertFullpath(const std::string& path)
{
#if _WIN32
    //assert(0); // TODO    
	return path;
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
