//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include "def.h"

struct KeyStatus {
	String name;
	unsigned int pressTimes = 0;
	bool isPressed = false;
	bool notDealt = false;

	[[nodiscard]] String toString() const noexcept {
		return L"name: " + name + L" pressTimes: " + std::to_wstring(pressTimes) + L" isPressed";
	}
};

class InteractManager {
	KeyStatus keyStatus[256];

public:
	explicit InteractManager();

	void update(const int keyCode, const bool isPressed) noexcept {
		if (keyCode >= 256) return;
		keyStatus[keyCode].isPressed = isPressed;
		if (isPressed) {
			keyStatus[keyCode].pressTimes++;
			keyStatus[keyCode].notDealt = true;
		}
		std::wcout << keyStatus[keyCode].toString() << std::endl;
	}
};

inline static InteractManager interactManager = InteractManager();
