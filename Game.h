//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include "Hud.h"
#include "InteractManager.h"
#include "Renderer.h"
#include "Window.h"

class Game {
	Hud hud = Hud();
	Window* window = nullptr;

public:
	explicit Game() = default;

	void render() const noexcept {
		if (window) window->render();
		hud.render();
	}

	void setWindow(Window* window) noexcept {
		if (this->window == window) return;
		if (this->window) this->window->onClose();
		this->window = nullptr;
		if (window && window->onOpen())
			this->window = window;
	}
};

inline static Game game = Game();
