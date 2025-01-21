//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include "Hud.h"
#include "Window.h"

class Game : public IRenderable {
	Hud hud = Hud();
	Window* window = nullptr;

public:
	explicit Game() = default;
	static void initialize() noexcept;
	void render() const noexcept override {
		if (window) window->render();
		hud.render();
	}

	int setWindow(Window* window) noexcept {
		if (this->window == window) Success();
		if (this->window) this->window->onClose();
		this->window = nullptr;
		if (window && window->onOpen()) {
			this->window = window;
			Success();
		}
		Failed();
	}

	[[nodiscard]] Window* getWindow() const noexcept { return window; }
};

inline static Game game = Game();
