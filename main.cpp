#include <Windows.h>
#include <iostream>

#include "../CarlbeksLib/gxdef.h"

__stdcall int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
 // create window
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszClassName = L"HighBloodPressure";
	RegisterClassEx(&wc);

	HWND hwnd = CreateWindowEx(0, L"HighBloodPressure", L"High Blood Pressure", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}
