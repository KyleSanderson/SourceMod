#ifndef _INCLUDE_SOURCEMOD_PLATFORM_H_
#define _INCLUDE_SOURCEMOD_PLATFORM_H_

#if defined WIN32 || defined WIN64
#define PLATFORM_WINDOWS
#if !defined WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#if !defined snprintf
#define snprintf _snprintf
#endif
#include <windows.h>
#include <direct.h>
#define PLATFORM_MAX_PATH		MAX_PATH
#else if defined __linux__
#define PLATFORM_LINUX
#define PLATFORM_POSIX
#include <dirent.h>
#include <errno.h>
#define PLATFORM_MAX_PATH		PATH_MAX
#endif

#endif //_INCLUDE_SOURCEMOD_PLATFORM_H_
