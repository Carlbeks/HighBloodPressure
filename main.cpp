
#include <Windows.h>
#include <minwindef.h>
#include <windef.h>
#include <wingdi.h>
#include <winuser.h>
#include <Uxtheme.h>
#include <iostream>
#include <dwmapi.h>

#include "hbp.h"
#include "InteractManager.h"

LRESULT __stdcall WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
			_LIKELY
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);// Paint caption
			EndPaint(hwnd, &ps);
			break;
		}
		case WM_LBUTTONDOWN: interactManager.update(VK_LBUTTON, true);
			break;
		case WM_RBUTTONDOWN: interactManager.update(VK_RBUTTON, true);
			break;
		case WM_LBUTTONUP: interactManager.update(VK_LBUTTON, false);
			break;
		case WM_RBUTTONUP: interactManager.update(VK_RBUTTON, false);
			break;
			_LIKELY
		case WM_NCHITTEST: {
			LRESULT lr = 0;
			BOOL r = DwmDefWindowProc(hwnd, uMsg, wParam, lParam, &lr);
			return lr;
		}
		case WM_MOUSEMOVE: { break; }
		case WM_MBUTTONDOWN: interactManager.update(VK_MBUTTON, true);
			break;
		case WM_MBUTTONUP: interactManager.update(VK_MBUTTON, false);
			break;
		case WM_COMMAND: std::wcout << L"WM_COMMAND" << std::endl;
			break;
		case WM_DWMCOMPOSITIONCHANGED: {
			MARGINS margins{
				.cxLeftWidth = 0,
				.cxRightWidth = 0,
				.cyTopHeight = 0,
				.cyBottomHeight = 0
			};
			HRESULT hr = RemoveDefaultCaption(hwnd, &margins);
			break;
		}
		case WM_NCCALCSIZE: if (wParam == 1) {
				NCCALCSIZE_PARAMS* pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
				pncsp->rgrc[0].left = pncsp->rgrc[0].left + 0;
				pncsp->rgrc[0].top = pncsp->rgrc[0].top + 0;
				pncsp->rgrc[0].right = pncsp->rgrc[0].right - 0;
				pncsp->rgrc[0].bottom = pncsp->rgrc[0].bottom - 0;
				return 0;
			}
		case WM_SIZE: switch (wParam) {
				case SIZE_RESTORED: break;
				case SIZE_MINIMIZED: break;
				case SIZE_MAXIMIZED: break;
				case SIZE_MAXSHOW:
				case SIZE_MAXHIDE:
				default: break;
			}
			break;
		case WM_ACTIVATE: {
			MARGINS margins{
				.cxLeftWidth = 0,
				.cxRightWidth = 0,
				.cyTopHeight = 0,
				.cyBottomHeight = 0
			};
			HRESULT hr = RemoveDefaultCaption(hwnd, &margins);
		}
		break;
			_UNLIKELY
		case WM_DESTROY: PostQuitMessage(0);
			return 0;
			_UNLIKELY
		case WM_CREATE: SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);// Force post NCCALCSIZE
			break;
		default: break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	AllocConsole();
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = L"None";
	wc.lpszClassName = ApplicationName.c_str();
	if (!RegisterClassExW(&wc)) return FALSE;
	MainInstance = hInstance;
	MainWindowHandle = CreateWindowExW(0, wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
	ShowWindow(MainWindowHandle, nCmdShow);
	Initialize();
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(109));
	MSG msg = { nullptr };
	while (GetMessageW(&msg, nullptr, 0, 0)) {
		if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
	DestroyAcceleratorTable(hAccelTable);
	return (int) msg.wParam;
}
