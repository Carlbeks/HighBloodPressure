
#include <Windows.h>
#include <winuser.h>

#include "../CarlbeksLib/gxdef.h"
#include "../CarlbeksLib/rgui.h"
#include "../CarlbeksLib/iwindows.h"

static HINSTANCE MainInstance;
static HWND MainWindowHandle;

static inline String ApplicationName = L"Project";

inline Carlbeks::UI::Main MainWindow;
inline Carlbeks::UI::TickThread MainTick{ &MainWindow };

inline void Initialize() noexcept {
	Carlbeks::WindowsInterface::ShowConsoleIO();
	MainWindow.setWindow(MainWindowHandle);
	MainWindow.setDC(GetDC(MainWindowHandle));
	MainTick.start();
}

inline void Finalize() noexcept {
	MainTick.stopAndWait();
	_wsystem(L"pause");
}
