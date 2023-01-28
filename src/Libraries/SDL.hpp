#pragma once

#if defined(__GNUC__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wdouble-promotion"
#endif

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_syswm.h>

#if defined(__GNUC__)
	#pragma GCC diagnostic pop
#endif
