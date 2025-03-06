
#include "includes.h"
#include "IText.h"
#include "TestCode.h"

LRESULT __stdcall WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
			[[likely]]
		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);
			if (renderer.checkResizing()) renderer.resizeShow();
			EndPaint(hwnd, &ps);
			break;
		}
			[[likely]]
		case WM_NCHITTEST: {
			POINT point = { GET_X_LPARAM(lParam), (GET_Y_LPARAM(lParam)) };
			ScreenToClient(hwnd, &point);
			const int xPos = point.x;
			const int yPos = point.y;
			if (renderer.windowSize != SIZE_MAXIMIZED) {
				if (xPos < interactSettings.actual.marginWidth) {
					if (yPos < interactSettings.actual.marginWidth) return HTTOPLEFT;
					if (renderer.getSyncHeight() - yPos < interactSettings.actual.marginWidth) return HTBOTTOMLEFT;
					return HTLEFT;
				}
				if (renderer.getSyncWidth() - xPos < interactSettings.actual.marginWidth) {
					if (yPos < interactSettings.actual.marginWidth) return HTTOPRIGHT;
					if (renderer.getSyncHeight() - yPos < interactSettings.actual.marginWidth) return HTBOTTOMRIGHT;
					return HTRIGHT;
				}
			}
			if (yPos < interactSettings.actual.marginWidth) return HTTOP;
			if (renderer.windowSize != SIZE_MAXIMIZED && renderer.getSyncHeight() - yPos < interactSettings.actual.marginWidth) return HTBOTTOM;
			if (yPos < interactSettings.actual.captionHeight) return HTCAPTION;
			LRESULT lr = 0;
			DwmDefWindowProc(hwnd, uMsg, wParam, lParam, &lr);
			return HTCLIENT;
		}
			[[likely]]
		case WM_MOUSEMOVE:
			interactManager.updateMouse(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			game.passEvent(MouseActionCode::MAC_MOVE, 0, interactManager.getMouseX(), interactManager.getMouseY());
			break;
			[[likely]]
		case WM_NCMOUSEMOVE: {
			POINT pt{ .x = GET_X_LPARAM(lParam), .y = GET_Y_LPARAM(lParam) };
			ScreenToClient(MainWindowHandle, &pt);
			interactManager.updateMouse(pt.x, pt.y);
			game.passEvent(MouseActionCode::MAC_MOVE, 0, interactManager.getMouseX(), interactManager.getMouseY());
			break;
		}
		case WM_SIZE:
			renderer.syncSize(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			switch (wParam) {
				case SIZE_RESTORED:
				case SIZE_MAXIMIZED:
					renderer.resize(
						renderer.getSyncWidth(),
						renderer.getSyncHeight()
					);
					PostMessageW(hwnd, WM_EXITSIZEMOVE, 0, 0);
					break;
				case SIZE_MINIMIZED:
				case SIZE_MAXSHOW:
				case SIZE_MAXHIDE:
				default:
					break;
			}
			renderer.windowSize = wParam;
			break;
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
		case WM_NCLBUTTONDOWN:
			if (interactManager.isInClientCaption()) {
				interactManager.update(VK_LBUTTON, true);
				game.passEvent(MouseActionCode::MAC_DOWN, interactManager.getMouseButtonCode() | static_cast<int>(MouseButtonCodeEnum::MBC_L_CHANGE), interactManager.getMouseX(), interactManager.getMouseY());
			}
			break;
		case WM_RBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
			if (interactManager.isInClientCaption()) {
				interactManager.update(VK_RBUTTON, true);
				game.passEvent(MouseActionCode::MAC_DOWN, interactManager.getMouseButtonCode() | static_cast<int>(MouseButtonCodeEnum::MBC_R_CHANGE), interactManager.getMouseX(), interactManager.getMouseY());
			}
			break;
		case WM_APP_LBUTTONUP: // case WM_LBUTTONUP: case WM_NCLBUTTONUP:
			if (interactManager.isInClientCaption()) {
				interactManager.update(VK_LBUTTON, false);
				game.passEvent(MouseActionCode::MAC_UP, interactManager.getMouseButtonCode() | static_cast<int>(MouseButtonCodeEnum::MBC_L_CHANGE), interactManager.getMouseX(), interactManager.getMouseY());
			}
			break;
		case WM_RBUTTONUP:
		case WM_NCRBUTTONUP:
			if (interactManager.isInClientCaption()) {
				interactManager.update(VK_RBUTTON, false);
				game.passEvent(MouseActionCode::MAC_UP, interactManager.getMouseButtonCode() | static_cast<int>(MouseButtonCodeEnum::MBC_R_CHANGE), interactManager.getMouseX(), interactManager.getMouseY());
			}
			break;
		case WM_APP_MBUTTONDOWN: // case WM_MBUTTONDOWN: case WM_NCMBUTTONDOWN:
			if (interactManager.isInClientCaption()) {
				if (wParam & 0x10) interactManager.update(VK_MBUTTON, true);
				game.passEvent(MouseActionCode::MAC_DOWN, interactManager.getMouseButtonCode() | static_cast<int>(MouseButtonCodeEnum::MBC_M_CHANGE), interactManager.getMouseX(), interactManager.getMouseY());
			}
			break;
		case WM_MBUTTONUP:
		case WM_NCMBUTTONUP:
			if (interactManager.isInClientCaption()) {
				interactManager.update(VK_MBUTTON, false);
				game.passEvent(MouseActionCode::MAC_UP, interactManager.getMouseButtonCode() | static_cast<int>(MouseButtonCodeEnum::MBC_M_CHANGE), interactManager.getMouseX(), interactManager.getMouseY());
			}
			break;
		case WM_MOUSEWHEEL:
			interactManager.update(VK_MBUTTON, true);
			interactManager.update(VK_MBUTTON, true);
			break;
		case WM_MOUSEHOVER:
			interactManager.mouseHover();
			break;
		case WM_NCMOUSELEAVE:
			interactManager.mouseLeaveCaption();
			if (!interactManager.isInWindow()) game.passEvent(MouseActionCode::MAC_LEAVE, 0, interactManager.getMouseX(), interactManager.getMouseY());
			break;
		case WM_MOUSELEAVE:
			interactManager.mouseLeaveClient();
			if (!interactManager.isInWindow()) game.passEvent(MouseActionCode::MAC_LEAVE, 0, interactManager.getMouseX(), interactManager.getMouseY());
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
				renderer.resizeStart();
				return 0;
			}
		case WM_EXITSIZEMOVE:
			renderer.resizeEnd();
			renderer.resize(renderer.getSyncWidth(), renderer.getSyncHeight());
			interactSettings.setScreenScale(static_cast<double>(GetSystemMetrics(SM_CYSCREEN)) / 2160.);
			break;
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
			[[unlikely]]
		case WM_DESTROY:
			PostQuitMessage(0);
			isRunning = false;
			return 0;
			[[unlikely]]
		case WM_CREATE:
			SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER); // Force post NCCALCSIZE
			renderer.resizeEnd();
			break;
		default:
			break;
	}
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

LRESULT __stdcall HookProc(int code, WPARAM wParam, LPARAM lParam) {
	if (code < 0) {
		Logger.log(L"HookProc nCode < 0");
		return CallNextHookEx(nullptr, code, wParam, lParam);
	}
	switch (const MSG* p = reinterpret_cast<MSG*>(lParam); p->message) {
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			PostMessageW(MainWindowHandle, WM_APP_LBUTTONUP, p->wParam, p->lParam);
			return 0;
		case WM_MBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
			PostMessageW(MainWindowHandle, WM_APP_MBUTTONDOWN, p->wParam, p->lParam);
			return 0;
		default:
			break;
	}
	return CallNextHookEx(nullptr, code, wParam, lParam);
}

void gameThread() {
	try {
		using namespace std::chrono;
		using Time = time_point<system_clock>;
		Time lastTick = system_clock::now();
		while (isRunning) {
			Time thisTime = system_clock::now();
			if (thisTime - lastTick < milliseconds(45)) {
				Sleep(1);
				continue;
			}
			game.tick();
			renderer.tick();
			lastTick = thisTime;
		}
	} catch (const Exception& e) { Logger.error(L"Game thread exception: " + e.getMessage()); }
	catch (const std::exception& e) { Logger.error(L"Game thread exception: " + atow(e.what())); }
	Logger.error(L"Game thread ended.");
	isRunning = false;
	DestroyWindow(MainWindowHandle);
}

void renderThread() {
	try {
		using namespace std::chrono;
		using Time = time_point<system_clock>;
		renderer.initialize();
		Time lastRender = system_clock::now();
		while (isRunning) {
			Time thisTime = system_clock::now();
			if (thisTime - lastRender < milliseconds(12)) {
				Sleep(1);
				continue;
			}
			game.render();
			lastRender = thisTime;
		}
	} catch (const Exception& e) { Logger.log(L"Render thread exception: " + e.getMessage()); }
	catch (const std::exception& e) { Logger.log(L"Render thread exception: " + atow(e.what())); }
	Logger.error(L"Render thread ended.");
	isRunning = false;
	DestroyWindow(MainWindowHandle);
}

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	game.setWindow(StartWindow::create()); // 次序提前至最先
	for (auto i : memoryManager.allocated) { Logger.print(L"  using", i.first, i.second.size, L"B", i.second.msg); }
	MainLogFile << L"--------Program Start--------" << std::endl;
	for (auto i : memoryManager.allocated) { MainLogFile << L"  using " << i.first << L" " << i.second.size << L"B " << i.second.msg << std::endl; }
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
	if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE)) Logger.print(L"SetProcessDpiAwarenessContext failed. LastError:", GetLastError());
	MainInstance = hInstance;
	MainWindowHandle = CreateWindowExW(0, wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
	ShowWindow(MainWindowHandle, nCmdShow);
	{
		interactManager.initialize();
	}
	SetConsoleOutputCP(65001);
	HHOOK hook = SetWindowsHookW(WH_GETMESSAGE, HookProc);
	test();
	HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCE(109));
	MSG msg = { nullptr };
	GameThread = Thread(gameThread);
	RenderThread = Thread(renderThread);
	while (GetMessageW(&msg, nullptr, 0, 0)) {
		if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
	DestroyAcceleratorTable(hAccelTable);
	isRunning = false;
	UnhookWindowsHookEx(hook);
	if (GameThread.joinable()) GameThread.join();
	if (RenderThread.joinable()) RenderThread.join();
	_wsystem(L"pause");
	for (auto i : memoryManager.allocated) { Logger.print(L"using", i.first, i.second.size, L"B", i.second.msg); }
	MainLogFile << L"-------- Program End --------" << std::endl;
	for (auto i : memoryManager.allocated) { MainLogFile << L"  using " << i.first << L" " << i.second.size << L"B " << i.second.msg << std::endl; }
	_wsystem(L"pause");
	_wsystem(L"pause");
	return static_cast<int>(msg.wParam);
}

struct Release {
	Release() = default;

	~Release() {
		MainLogFile << L"--------- Last Check ---------" << std::endl;
		for (auto i : memoryManager.allocated) { MainLogFile << L"  using " << i.first << L" " << i.second.size << L"B " << i.second.msg << std::endl; }
		MainLogFile.close();
		delete &MainLogFile;
	}
} x;
