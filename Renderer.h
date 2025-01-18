//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include "def.h"

/**
 * 用于标记相对位置。
 */
enum class Location { LEFT_TOP, LEFT, LEFT_BOTTOM, TOP, CENTER, BOTTOM, RIGHT_TOP, RIGHT, RIGHT_BOTTOM };

interface IRenderable {
	virtual ~IRenderable() = default;
	virtual void render() const noexcept = 0;
};

class Renderer {
	inline static HDC MainDC;
	int windowWidth = 0, windowHeight = 0;

public:
	static void initialize() noexcept;
	explicit Renderer() = default;

	void resize(int width, int height) noexcept(false);

	[[nodiscard]] int getWidth() const noexcept { return windowWidth; }
	[[nodiscard]] int getHeight() const noexcept { return windowHeight; }
};

inline static Renderer renderer = Renderer();
