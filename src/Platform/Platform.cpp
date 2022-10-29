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
// X11 - Window (unsigned long)
typedef unsigned long WindowHandle;
#elif defined(__APPLE__)
// MacOS Cocoa - NSWindow / NSView (void*)
using WindowHandle = void*;
#endif

WindowHandle getPlatformWindowHandle(SDL_Window* inWindow)
{
	SDL_SysWMinfo systemInfo;
	SDL_VERSION(&systemInfo.version);

	if (SDL_GetWindowWMInfo(inWindow, &systemInfo))
	{
#if defined(_WIN32)
		assert(systemInfo.subsystem == SDL_SYSWM_WINDOWS && "subsystem not windows");
		return systemInfo.info.win.window;
#elif defined(__APPLE__)
		assert(systemInfo.subsystem == SDL_SYSWM_COCOA && "subsystem not macos");
		return systemInfo.info.cocoa.window;
#elif defined(__linux__)
		assert((systemInfo.subsystem == SDL_SYSWM_X11 || systemInfo.subsystem == SDL_SYSWM_WAYLAND) && "subsystem not linux");

	#if defined(SDL_VIDEO_DRIVER_X11)
		return systemInfo.info.x11.window;
	#elif defined(SDL_VIDEO_DRIVER_WAYLAND)
		return systemInfo.info.wl.shell_surface;
	#else
		std::cerr << "Platform handle not recognized";
	#endif
#endif
	}
	else
	{
		std::cerr << "Couldn't get window information: " << SDL_GetError();
	}
	return 0;
}
}

/*****************************************************************************/
void Platform::initialize(SDL_Window* inWindow)
{
#if defined(_WIN32)
	auto handle = getPlatformWindowHandle(inWindow);
	util::WindowsPlatform platform;
	platform.setIcon(handle);
#else
	UNUSED(inWindow);
#endif
}