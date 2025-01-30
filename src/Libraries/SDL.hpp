#pragma once

#if defined(__GNUC__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wdouble-promotion"
	#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

#if defined(__linux__)
	// #define SDL_VIDEO_DRIVER_X11
	#define SDL_VIDEO_DRIVER_WAYLAND
#endif

#define SDL_MAIN_HANDLED
#include <SDL.h>

#if defined(__GNUC__)
	#pragma GCC diagnostic pop
#endif
