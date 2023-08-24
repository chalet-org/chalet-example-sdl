#ifdef _WIN32
	#include "Platform/Win32/WindowsPlatform.hpp"

	#include "Resource.h"

	#include <dwmapi.h>

	#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
		#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
	#endif

	#include <cmath>

namespace util
{
WindowsPlatform::WindowsPlatform()
{
	m_screenScalingFactor = getScreenScalingFactor();
}

/*****************************************************************************/
// The window handle uses 32x32 (ICON_BIG) & 16x16 (ICON_SMALL) sized icons.
// This should be called any time the window is create/recreated
//
void WindowsPlatform::initialize(HWND handle)
{
	// Get the icon directory
	PBYTE iconDirectory = getIconDirectory(WIN32_ICON_MAIN);
	std::array<int, 5> icons = { 16, 32, 48, 64, 128 };

	size_t indexSmallIcon = static_cast<size_t>(std::min<float>(std::max<float>(std::ceil(m_screenScalingFactor - 1.0f), 0.0f), static_cast<float>(icons.size()) - 1.0f));
	size_t indexBigIcon = static_cast<size_t>(std::min<float>(std::max<float>(std::ceil(m_screenScalingFactor - 1.0f), 0.0f) + 1.0f, static_cast<float>(icons.size()) - 1.0f));

	HICON smallIcon = getIconFromIconDirectory(iconDirectory, icons[indexSmallIcon]);
	HICON bigIcon = getIconFromIconDirectory(iconDirectory, icons[indexBigIcon]);

	if (handle && smallIcon)
		SendMessage(handle, WM_SETICON, ICON_SMALL, (LPARAM)smallIcon);

	if (handle && bigIcon)
		SendMessage(handle, WM_SETICON, ICON_BIG, (LPARAM)bigIcon);

	// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-destroyicon
	// It is only necessary to call DestroyIcon for icons and cursors created with the following functions:
	//   CreateIconFromResourceEx (if called without the LR_SHARED flag)

	// Dark mode
	// Note: in MinGW (GCC 12.2 at least), the titlebar is noticeably smaller in Windows 11 than if the app were compiled with MSVC
	//
	BOOL value = TRUE;
	::DwmSetWindowAttribute(handle, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));

	::UpdateWindow(handle);
}

/*****************************************************************************/
// Gets the screen scaling factor of the device from the supplied handle
//
float WindowsPlatform::getScreenScalingFactor()
{
	if (m_screenScalingFactor != 0.0f)
		return m_screenScalingFactor;

	HDC screenDC = GetDC(nullptr);
	int logicalScreenHeight = GetDeviceCaps(screenDC, VERTRES);
	int physicalScreenHeight = GetDeviceCaps(screenDC, DESKTOPVERTRES);
	m_screenScalingFactor = static_cast<float>(physicalScreenHeight) / static_cast<float>(logicalScreenHeight);
	ReleaseDC(nullptr, screenDC);

	return m_screenScalingFactor;
}

/*****************************************************************************/
// Loads a .ico file from The application's resources, and can contain multiple
// sizes (for instance 16x16, 32x32 & 64x64). This is referred to as an
// "Icon Directory". Additionally, it can have a single icon
//
PBYTE WindowsPlatform::getIconDirectory(const int inResourceId)
{
	HMODULE hModule = GetModuleHandle(nullptr);
	HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(inResourceId), RT_GROUP_ICON);

	HGLOBAL hData = LoadResource(hModule, hResource);
	PBYTE data = (PBYTE)LockResource(hData);

	return data;
}

/*****************************************************************************/
// This will attempt to load a single icon from an icon directory
// If the requested size isn't found, the first one is returned
//
HICON WindowsPlatform::getIconFromIconDirectory(PBYTE inIconDirectory, const uint32_t inSize)
{
	HMODULE hModule = GetModuleHandle(nullptr);
	int resourceId = LookupIconIdFromDirectoryEx(inIconDirectory, TRUE, inSize, inSize, LR_DEFAULTCOLOR | LR_SHARED);
	HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(resourceId), RT_ICON);

	HGLOBAL hData = LoadResource(hModule, hResource);
	PBYTE data = (PBYTE)LockResource(hData);
	DWORD sizeofData = SizeofResource(hModule, hResource);

	HICON icon = CreateIconFromResourceEx(data, sizeofData, TRUE, 0x00030000, inSize, inSize, LR_DEFAULTCOLOR | LR_SHARED);
	return icon;
}
}

#endif // _WIN32
