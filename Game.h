//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include "Hud.h"
#include "Window.h"

class Game : public IRenderable {
	friend void gameThread();
	Hud hud = Hud();
	Window* window = nullptr;
	FloatWindow* floatWindow;
	QWORD currentTick = 0;
public:
	explicit Game();
	~Game() override;
	static void initialize() noexcept;

	void render() const noexcept override {
		renderer.gameStartRender();
		hud.render();
		if (window) window->render();
		if (floatWindow) floatWindow->render();
		renderer.gameEndRender();
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
	[[nodiscard]] QWORD getTick() const noexcept { return currentTick; }

	void tick() noexcept {
		++currentTick;
		hud.tick();
		if (window) window->tick();
		if (floatWindow) floatWindow->tick();
	}
};

extern Game game;
