
#include <Windows.h>
#include <consoleapi.h>
#include <corecrt_wprocess.h>
#include <minwindef.h>
#include <windef.h>
#include <wingdi.h>
#include <winuser.h>
#include <Uxtheme.h>
#include <vssym32.h>

#include "../CarlbeksLib/iwindows.h"

#include "core.h"

LRESULT __stdcall WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		[[likely]] case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps); // Paint caption
			MainWindow.fireRender();
			EndPaint(hwnd, &ps);
			break;
		}
		case WM_LBUTTONDOWN:
			MainWindow.fireMouseDown(Carlbeks::UI::MA_LEFT);
			MainWindow.fireRender();
			break;
		case WM_RBUTTONDOWN:
			MainWindow.fireMouseDown(Carlbeks::UI::MA_RIGHT);
			MainWindow.fireRender();
			break;
		case WM_LBUTTONUP:
			MainWindow.fireMouseUp(Carlbeks::UI::MA_LEFT);
			MainWindow.fireRender();
			break;
		case WM_RBUTTONUP:
			MainWindow.fireMouseUp(Carlbeks::UI::MA_RIGHT);
			MainWindow.fireRender();
			break;
		[[likely]] case WM_NCHITTEST: {
			Carlbeks::UI::UIReturn ret = MainWindow.fireMouseMove();
			MainWindow.fireRender();
			if (ret & 0x20) return ret ^ 0x20;
			return HTCLIENT;
			LRESULT lr = 0;
			BOOL r = DwmDefWindowProc(hwnd, uMsg, wParam, lParam, &lr);
			return lr;
		}
		[[likely]] case WM_MOUSEMOVE: {
			MainWindow.fireMouseMove();
			// MainWindow.fireRender();
			break;
		}
		[[unlikely]] case WM_MBUTTONDOWN:
			MainWindow.fireMouseDown(Carlbeks::UI::MA_MID);
			MainWindow.fireRender();
			break;
		[[unlikely]] case WM_MBUTTONUP:
			MainWindow.fireMouseUp(Carlbeks::UI::MA_MID);
			MainWindow.fireRender();
			break;
		[[unlikely]] case WM_COMMAND:
			std::wcout << L"WM_COMMAND" << std::endl;
			break;
		case WM_DWMCOMPOSITIONCHANGED: {
			MARGINS margins{
				.cxLeftWidth = 0,
				.cxRightWidth = 0,
				.cyTopHeight = 0,
				.cyBottomHeight = 0
			};
			HRESULT hr = Carlbeks::WindowsInterface::RemoveDefaultCaption(hwnd, &margins);
			break;
		}
		case WM_NCCALCSIZE:
			if (wParam == 1) {
				NCCALCSIZE_PARAMS* pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
				pncsp->rgrc[0].left = pncsp->rgrc[0].left + 0;
				pncsp->rgrc[0].top = pncsp->rgrc[0].top + 0;
				pncsp->rgrc[0].right = pncsp->rgrc[0].right - 0;
				pncsp->rgrc[0].bottom = pncsp->rgrc[0].bottom - 0;
				return 0;
			}
		case WM_SIZE:
			MainWindow.fireResize();
			MainWindow.fireRender();
			break;
		case WM_ACTIVATE: {
			MARGINS margins{
				.cxLeftWidth = 0,
				.cxRightWidth = 0,
				.cyTopHeight = 0,
				.cyBottomHeight = 0
			};
			HRESULT hr = Carlbeks::WindowsInterface::RemoveDefaultCaption(hwnd, &margins);
		} break;
		[[unlikely]] case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		[[unlikely]] case WM_CREATE:
			SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER); // Force post NCCALCSIZE
			break;
		default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = L"None";
	wc.lpszClassName = ApplicationName.c_str();
	if (!RegisterClassExW(&wc)) return FALSE;
	MainInstance = hInstance;
	MainWindowHandle = CreateWindowExW(0, wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(MainWindowHandle, nCmdShow);
	Initialize();
	MainWindow.fireResize();
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(109));
	MSG msg = { 0 };
	while (GetMessageW(&msg, NULL, 0, 0)) {
		if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
	DestroyAcceleratorTable(hAccelTable);
	Finalize();
	return (int) msg.wParam;
}
