
#include "includes.h"

LRESULT __stdcall WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
			_LIKELY
		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			break;
		}
			_LIKELY
		case WM_NCHITTEST: {
			POINT point = { GET_X_LPARAM(lParam), (GET_Y_LPARAM(lParam)) };
			ScreenToClient(hwnd, &point);
			const int xPos = point.x;
			const int yPos = point.y;
			if (xPos < 30) {
				if (yPos < 30) return HTTOPLEFT;
				if (renderer.getHeight() - yPos < 30) return HTBOTTOMLEFT;
				return HTLEFT;
			}
			if (renderer.getWidth() - xPos < 30) {
				if (yPos < 30) return HTTOPRIGHT;
				if (renderer.getHeight() - yPos < 30) return HTBOTTOMRIGHT;
				return HTBOTTOM;
			}
			if (yPos < 30) return HTTOP;
			if (renderer.getHeight() - yPos < 30) return HTBOTTOM;
			if (yPos < 240) return HTCAPTION;
			LRESULT lr = 0;
			DwmDefWindowProc(hwnd, uMsg, wParam, lParam, &lr);
			return HTCLIENT;
		}
		case WM_KEYDOWN:
			interactManager.update(static_cast<int>(wParam), true);
			break;
		case WM_KEYUP:
			interactManager.update(static_cast<int>(wParam), false);
			break;
		case WM_SYSKEYDOWN:
			interactManager.update(static_cast<int>(wParam), true);
			break;
		case WM_SYSKEYUP:
			interactManager.update(static_cast<int>(wParam), false);
			break;
		case WM_LBUTTONDOWN:
			interactManager.update(VK_LBUTTON, true);
			break;
		case WM_RBUTTONDOWN:
			interactManager.update(VK_RBUTTON, true);
			break;
		case WM_LBUTTONUP:
			interactManager.update(VK_LBUTTON, false);
			break;
		case WM_RBUTTONUP:
			interactManager.update(VK_RBUTTON, false);
			break;
		case WM_MOUSEMOVE:
			interactManager.updateMouse(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_MBUTTONDOWN:
			if (wParam & 0x10) interactManager.update(VK_MBUTTON, true);
			break;
		case WM_MBUTTONUP:
			interactManager.update(VK_MBUTTON, false);
			break;
		case WM_MOUSEWHEEL:
			interactManager.update(VK_MBUTTON, true);
			interactManager.update(VK_MBUTTON, true);
			break;
		case WM_MOUSEHOVER:
			// case WM_NCMOUSEHOVER:
			interactManager.mouseHover();
			break;
		case WM_MOUSELEAVE:
			// case WM_NCMOUSELEAVE:
			interactManager.mouseLeave();
			break;
		case WM_COMMAND:
			std::wcout << L"WM_COMMAND" << std::endl;
			break;
		case WM_DWMCOMPOSITIONCHANGED: {
			MARGINS margins{
				.cxLeftWidth = 0,
				.cxRightWidth = 0,
				.cyTopHeight = 0,
				.cyBottomHeight = 0
			};
			RemoveDefaultCaption(hwnd, &margins);
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
		case WM_SIZE: {
			renderer.resize(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			switch (wParam) {
				case SIZE_RESTORED:
				case SIZE_MINIMIZED:
				case SIZE_MAXIMIZED:
				case SIZE_MAXSHOW:
				case SIZE_MAXHIDE:
				default:
					break;
			}
			break;
		}
		case WM_ACTIVATE: {
			constexpr MARGINS margins{
				.cxLeftWidth = 0,
				.cxRightWidth = 0,
				.cyTopHeight = 0,
				.cyBottomHeight = 0
			};
			HRESULT hr = RemoveDefaultCaption(hwnd, &margins);
		}
		break;
			_UNLIKELY
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			_UNLIKELY
		case WM_CREATE:
			SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);// Force post NCCALCSIZE
			break;
		default:
			break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	ShowConsoleIO();
	WNDCLASSEX wc = {};
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
	if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE)) std::wcout << L"SetProcessDpiAwarenessContext failed. LastError: " << GetLastError() << std::endl;
	MainInstance = hInstance;
	MainWindowHandle = CreateWindowExW(0, wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
	ShowWindow(MainWindowHandle, nCmdShow);
	InteractManager::initialize();
	Renderer::initialize();
	HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCE(109));
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
