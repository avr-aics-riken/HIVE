/**
 * @file Time.h
 * プラットフォーム別Time関数
 */
#ifndef INCLUDE_TIME_H
#define INCLUDE_TIME_H

#if _WIN32
#include <Windows.h>
#else // other platform
#include <sys/time.h>
#endif

#if _WIN32

inline double GetTimeCount()
{
	//ULONGLONG tm = GetTickCount64();// upper Vista
	static MMRESULT r = timeBeginPeriod(1); // run once.
	const DWORD tm = timeGetTime();
	return static_cast<f64>(tm) * 1e-3;
}

#else // other OS

inline double GetTimeCount()
{
    timeval timet;
    gettimeofday(&timet,0);
    return timet.tv_sec + static_cast<double>(timet.tv_usec) * 1e-6;
}

#endif // OS

#endif // INCLUDE_TIME_H

