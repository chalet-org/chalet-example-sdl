#ifndef PCH_HPP
#define PCH_HPP

#ifndef _DEBUG
	#ifndef NDEBUG
		#define NDEBUG
	#endif
#endif // _DEBUG

#if defined(__linux__) && !defined(SDL_VIDEO_DRIVER_X11)
	#define SDL_VIDEO_DRIVER_X11
#endif

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_syswm.h>

#include <algorithm>
#include <cstdio>
#include <deque>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

// Windows
#ifdef _WIN32
	#ifndef UNICODE
		#define UNICODE
	#endif

	#ifndef _UNICODE
		#define _UNICODE
	#endif

	// #define WIN32_LEAN_AND_MEAN
	// #include <windows.h>
#endif // _WIN32

// Macros
#define UNUSED(x) (void)(x)

#endif // PCH_HPP
