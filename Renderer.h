//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include "def.h"

class Game;
/**
 * 用于标记相对位置。
 */
enum class Location : char { LEFT_TOP, LEFT, LEFT_BOTTOM, TOP, CENTER, BOTTOM, RIGHT_TOP, RIGHT, RIGHT_BOTTOM };

interface IRenderable {
	virtual ~IRenderable() = default;
	virtual void render() const noexcept = 0;
};

interface ITickable {
	virtual ~ITickable() = default;
	virtual void tick() noexcept = 0;
};

class Renderer : public ITickable {
	/**
	 * 后续可能用不到，可能可删
	 */
	Game* pGame = nullptr;
	inline static HDC MainDC;
	int windowWidth = 0, windowHeight = 0;
	int resizeTime = 0;

public:
	static void initialize() noexcept;
	explicit Renderer() = default;
	void resize(int width, int height) noexcept(false);
	void setGame(Game& game) noexcept { pGame = &game; }
	[[nodiscard]] int getWidth() const noexcept { return windowWidth; }
	[[nodiscard]] int getHeight() const noexcept { return windowHeight; }

	void tick() noexcept override {
		// 联合处理resize防抖
		if (resizeTime > 0) --resizeTime;
		else if (resizeTime == 0) {
			resize(windowWidth, windowHeight);
			resizeTime = -1;
		}
	}
};

inline static Renderer renderer = Renderer();
