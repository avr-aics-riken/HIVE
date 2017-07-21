/**
 * @file sleep.h
 * sleep class
 */

#pragma once

#ifdef _WIN32

#include <windows.h>

inline void os_sleep (int milliseconds) {
    Sleep (milliseconds);
}

#else

#include <time.h>

inline void os_sleep (int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = milliseconds % 1000 * 1000000;
    nanosleep (&ts, NULL);
}

#endif
