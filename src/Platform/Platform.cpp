#include "Platform/Platform.hpp"

#include <cassert>

#if defined(_WIN32)
	#include "Platform/Win32/WindowsPlatform.hpp"
#endif

namespace
{
#if defined(_WIN32)
using WindowHandle = HWND__*;
#elif defined(__linux__)
	#if defined(SDL_VIDEO_DRIVER_WAYLAND)
using WindowHandle = void*;
	#else
// X11 - Window (unsigned long)
using WindowHandle = unsigned long;
	#endif
#elif defined(__APPLE__)
// MacOS Cocoa - NSWindow / NSView (void*)
using WindowHandle = void*;
#elif defined(__EMSCRIPTEN__)
using WindowHandle = void*;
#endif

WindowHandle getPlatformWindowHandle(SDL_Window* inWindow)
{
#if defined(_WIN32)
	HWND hwnd = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(inWindow), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
	return hwnd;
#elif defined(__APPLE__)
	auto handle = SDL_GetPointerProperty(SDL_GetWindowProperties(inWindow), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, NULL);
	return handle;
#elif defined(__linux__)
	if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0)
	{
		// Display* xdisplay = (Display*)SDL_GetPointerProperty(SDL_GetWindowProperties(inWindow), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
		auto xwindow = (Window)SDL_GetNumberProperty(SDL_GetWindowProperties(inWindow), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
		return xwindow;
	}
	else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0)
	{
		// struct wl_display* display = (struct wl_display*)SDL_GetPointerProperty(SDL_GetWindowProperties(inWindow), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
		auto surface = (wl_surface*)SDL_GetPointerProperty(SDL_GetWindowProperties(inWindow), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL);
		return surface;
	}
	else
	{
		std::cerr << "Couldn't get window information: " << SDL_GetError();
		return 0;
	}
#else
	{
		std::cerr << "Couldn't get window information: " << SDL_GetError();
		return 0;
	}
#endif
}
}

/*****************************************************************************/
void Platform::initialize(SDL_Window* inWindow)
{
	auto handle = getPlatformWindowHandle(inWindow);
#if defined(_WIN32)
	util::WindowsPlatform platform;
	platform.initialize(handle);
#else
	UNUSED(handle);
#endif
}