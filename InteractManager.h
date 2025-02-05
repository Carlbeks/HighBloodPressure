//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include "def.h"
#include "hbp.h"

struct KeyStatus {
	String name;
	unsigned int pressTimes = 0;
	bool pressed = false;
	bool notDealt = false;

	[[nodiscard]] bool isPressed() const noexcept { return pressed; }
	[[nodiscard]] unsigned int wasPressed() const noexcept { return pressTimes; }

	void deals() noexcept {
		notDealt = false;
		pressTimes = 0;
	}

	[[nodiscard]] String toString() const noexcept { return L"KeyStatus: { name = \"" + name + L"\"; pressTimes = " + std::to_wstring(pressTimes) + L"; pressed = " + (pressed ? L"true; }" : L"false; }"); }
};

struct MouseStatus {
	String name;
	unsigned int pressTimes = 0;
	bool pressed = false;
	bool notDealt = false;
	bool longHold = false;

	[[nodiscard]] bool isPressed() const noexcept { return pressed; }
	[[nodiscard]] unsigned int wasPressed() const noexcept { return pressTimes; }

	void deals() noexcept {
		notDealt = false;
		pressTimes = 0;
	}

	[[nodiscard]] String toString() const noexcept { return L"MouseStatus: { name = \""+ name + L"\"; pressTimes = " + std::to_wstring(pressTimes) + L"; pressed = " + (pressed ? L"true, longHold = " : L"false, longHold = ") + (longHold ? L"true; }" : L"false; }") ; }
};

struct KeyBinding;

class InteractManager {
	inline static TRACKMOUSEEVENT trackMouseEvent{
		.cbSize = sizeof(TRACKMOUSEEVENT),
		.dwFlags = TME_HOVER | TME_LEAVE | TME_NONCLIENT,
		.hwndTrack = nullptr,
		.dwHoverTime = 0
	};
	KeyStatus keyStatus[256];
	int mouseX = 0, mouseY = 0;
	int mouseWheel = 0;
	int rebindResult = 0;
	bool rebinding = false;
	bool inWindow = false;// 鼠标是否在窗口内部
	bool hovering = false;

public:
	static void initialize() noexcept { trackMouseEvent.hwndTrack = MainWindowHandle; }
	explicit InteractManager();

	void update(const int keyCode, const bool isPressed) noexcept {
		if (keyCode >= 256) return;
		if (rebinding) {
			rebindResult = keyCode;
			return;
		}
		keyStatus[keyCode].pressed = isPressed;
		if (isPressed) {
			keyStatus[keyCode].pressTimes++;
			keyStatus[keyCode].notDealt = true;
		}
	}

	void updateMouse(const int x, const int y) noexcept {
		mouseX = x;
		mouseY = y;
		inWindow = true;
		hovering = false;
		if (!TrackMouseEvent(&trackMouseEvent)) std::wcout << L"TrackMouseEvent failed. LastError: " << GetLastError() << std::endl;
	}

	void mouseHover() noexcept {
		hovering = true;
		inWindow = true;
		std::wcout << L"hover" << std::endl;
	}

	void mouseLeave() noexcept {
		inWindow = false;
		hovering = false;
		std::wcout << L"leave" << std::endl;
	}

	void updateWheel(const int wheel) noexcept { mouseWheel += wheel; }
	[[nodiscard]] int getMouseX() const noexcept { return mouseX; }
	[[nodiscard]] int getMouseY() const noexcept { return mouseY; }
	[[nodiscard]] int getMouseWheel() const noexcept { return mouseWheel; }
	[[nodiscard]] bool isHovering() const noexcept { return hovering; }
	[[nodiscard]] bool isInWindow() const noexcept { return inWindow; }
	KeyStatus& getKey(const int keyCode) noexcept { return keyStatus[keyCode]; }
	KeyStatus& getKey(const KeyBinding& binding) noexcept;

	int dealMouseWheel() noexcept {
		const int ret = mouseWheel;
		mouseWheel = 0;
		return ret;
	}
};

inline static InteractManager interactManager = InteractManager();

struct KeyBinding {
	String id;
	int keyCode;
	[[nodiscard]] bool isPressed() const noexcept { return interactManager.getKey(keyCode).isPressed(); }
	[[nodiscard]] unsigned int wasPressed() const noexcept { return interactManager.getKey(keyCode).wasPressed(); }
	void deals() const noexcept { interactManager.getKey(keyCode).deals(); }
};
