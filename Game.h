//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include <random>

#include "gc.h"
#include "Hud.h"
#include "Task.h"
#include "Window.h"

class Game final /* : public IRenderable, public ITickable */ {
	friend void gameThread();
	WindowManager windows;
	Hud hud = Hud(); // 8
	CaptionWindow* caption; // 8
	FloatWindow* floatWindow; // 8
	QWORD currentTick = 0; // 8

public:
	TaskScheduler tasks; // 8
	std::minstd_rand random;

	Game();
	~Game();

	[[nodiscard]] QWORD getTick() const noexcept { return currentTick; }
	int closeWindow(Window* const window) noexcept { return windows.pop(window); }

	void render() const noexcept {
		if (renderer.checkResizing()) return;
		renderer.gameStartRender();
		caption->render();
		hud.render();
		windows.render();
		floatWindow->render();
		renderer.gameEndRender();
		gc.pack();
	}

	/**
	 * 所有窗口都提交给Game保管，在适当时刻自动删除。
	 * 建议WindowType::create
	 */
	int setWindow(Window* window) noexcept {
		if (window) {
			if (window->onOpen()) {
				for (Window& i : windows) i.passEvent(MouseActionCode::MAC_LEAVE, 0, 0, 0);
				windows.pushNewed(window);
				Success();
			}
			Failed();
		}
		windows.clear();
		Success();
	}

	[[nodiscard]] FloatWindow& getFloatWindow() const noexcept { return *floatWindow; }

	[[nodiscard]] Window* getWindow() const noexcept {
		if (auto *const back = windows.back()) return dynamic_cast<Window*>(back);
		Logger.error(L"Game::getWindow returns nullptr");
		return nullptr;
	}

	void tick() noexcept {
		++currentTick;
		floatWindow->clear();
		floatWindow->tick();
		caption->tick();
		hud.tick();
		windows.tick();
		floatWindow->update();
		tasks.runAll();
		gc.collect();
	}

	void handleResize() {
		caption->onResize();
		hud.onResize();
		windows.onResize();
		floatWindow->onResize();
	}

	void passEvent(const MouseActionCode action, const MouseButtonCode value, const int x, const int y) const noexcept {
		caption->passEvent(action, value, x, y);
		if (Window* const window = getWindow()) window->passEvent(action, value, x, y);
		floatWindow->passEvent(action, value, x, y);
	}
};

extern Game game;
