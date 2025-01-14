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

inline HRESULT RemoveDefaultCaption(HWND hWnd, const MARGINS* p) noexcept { return DwmExtendFrameIntoClientArea(hWnd, p); }

inline void ShowConsoleIO() noexcept {
	AllocConsole();
	FILE* pCout;
	freopen_s(&pCout, "CONOUT$", "w", stdout);
	FILE* pCin;
	freopen_s(&pCin, "CONIN$", "r", stdin);
}

inline const String ApplicationName = L"Hyblud Presher";
inline HINSTANCE MainInstance;
inline HWND MainWindowHandle;
