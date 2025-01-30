#pragma once

#include <array>

#include "Libraries/Win32.hpp"

namespace util
{
struct WindowsPlatform
{
	WindowsPlatform();

	void initialize(HWND inHandle);

private:
	float getScreenScalingFactor();

	PBYTE getIconDirectory(const int inResourceId);
	HICON getIconFromIconDirectory(PBYTE inIconDirectory, const uint32_t inSize);

	float m_screenScalingFactor = 0.0f;
};
}
