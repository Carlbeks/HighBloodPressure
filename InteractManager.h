//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include "def.h"
#include "exception.h"
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

	[[nodiscard]] String toString() const noexcept { return L"MouseStatus: { name = \"" + name + L"\"; pressTimes = " + std::to_wstring(pressTimes) + L"; pressed = " + (pressed ? L"true, longHold = " : L"false, longHold = ") + (longHold ? L"true; }" : L"false; }"); }
};

struct KeyBinding;

class InteractManager {
	TRACKMOUSEEVENT trackMouseEvent{
		.cbSize = sizeof(TRACKMOUSEEVENT),
		.dwFlags = TME_HOVER | TME_LEAVE,
		.hwndTrack = nullptr,
		.dwHoverTime = HOVER_DEFAULT
	};
	KeyStatus keyStatus[256];
	int mouseX = 0, mouseY = 0;
	int mouseWheel = 0;
	int rebindResult = 0;
	char outsideWindow = 0;// 鼠标是否在窗口外部。1位：在客户区；2位：在标题栏
	bool rebinding = false;
	bool hovering = false;

public:
	void initialize() noexcept { trackMouseEvent.hwndTrack = MainWindowHandle; }
	InteractManager();

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


	void mouseLeaveCaption() noexcept {
		outsideWindow &= ~2;
		hovering = false;
	}

	void mouseLeaveClient() noexcept {
		outsideWindow &= ~1;
		hovering = false;
	}

	void updateMouse(int x, int y) noexcept;
	void updateWheel(const int wheel) noexcept { mouseWheel += wheel; }
	void mouseHover() noexcept { hovering = true; }
	[[nodiscard]] int getMouseX() const noexcept { return mouseX; }
	[[nodiscard]] int getMouseY() const noexcept { return mouseY; }
	[[nodiscard]] int getMouseWheel() const noexcept { return mouseWheel; }
	[[nodiscard]] bool isHovering() const noexcept { return hovering; }
	[[nodiscard]] bool isInWindow() const noexcept { return outsideWindow; }
	[[nodiscard]] bool isInSizeBox() const noexcept;
	[[nodiscard]] bool isInClientCaption() const noexcept;
	[[nodiscard]] KeyStatus& getKey(const int keyCode) noexcept { return keyStatus[keyCode]; }
	[[nodiscard]] KeyStatus& getKey(const KeyBinding& binding) noexcept;
	[[nodiscard]] unsigned int /*MouseButtonCode*/ getMouseButtonCode() const noexcept;

	int dealMouseWheel() noexcept {
		const int ret = mouseWheel;
		mouseWheel = 0;
		return ret;
	}
};

inline InteractManager interactManager = InteractManager();

struct KeyBinding {
	String id;
	int keyCode;
	[[nodiscard]] bool isPressed() const noexcept { return interactManager.getKey(keyCode).isPressed(); }
	[[nodiscard]] unsigned int wasPressed() const noexcept { return interactManager.getKey(keyCode).wasPressed(); }
	void deals() const noexcept { interactManager.getKey(keyCode).deals(); }
};

class InteractSettings {
	struct Options {
		int captionHeight = 120;
		int marginWidth = 40;
		int fontHeight = 96;
		int floatWindowMargin = 16;
	};

	struct Constants {
		/**
		 * 非常重要的设置项。<br>
		 * 管理ui的缩放比例。
		 */
		double uiScale = 1;
		double screenScale = 1;
		unsigned int floatWindowBackground = 0xdd000000;
	};

public:
	Options options;
	Options actual;
	Constants constants;

	InteractSettings& setUiScale(const double scale) noexcept {
		constants.uiScale = scale;
		actual.fontHeight = static_cast<int>(options.fontHeight * scale);
		return *this;
	}

	InteractSettings& setScreenScale(const double scale) noexcept {
		constants.screenScale = scale;
		actual.captionHeight = static_cast<int>(options.captionHeight * scale);
		actual.marginWidth = static_cast<int>(options.marginWidth * scale);
		return *this;
	}
};

inline InteractSettings interactSettings = InteractSettings();
