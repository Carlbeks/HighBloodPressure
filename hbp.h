#pragma once

#include "def.h"

inline BOOL NewProcess(const String& cmdline) noexcept {
	STARTUPINFOW si = {
		sizeof(si), nullptr, nullptr, nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr, nullptr, nullptr, nullptr
	};
	PROCESS_INFORMATION pi{
		nullptr, nullptr, 0, 0
	};
	return CreateProcessW(nullptr, const_cast<wchar*>(cmdline.c_str()), nullptr, nullptr, 0, 0, nullptr, nullptr, &si, &pi);
}

inline HRESULT RemoveDefaultCaption(const HWND hWnd, const MARGINS* p) noexcept { return DwmExtendFrameIntoClientArea(hWnd, p); }

inline bool ShowConsoleIO() noexcept {
	AllocConsole();
	FILE* pCout;
	freopen_s(&pCout, "CONOUT$", "w", stdout);
	FILE* pCin;
	freopen_s(&pCin, "CONOUT$", "r+", stdin);
	return true;
}

inline const String ApplicationName = L"High Blood Pressure";
inline HINSTANCE MainInstance;
inline HWND MainWindowHandle;
inline Thread GameThread;
inline Thread RenderThread;
inline bool isRunning = ShowConsoleIO();
