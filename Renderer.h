//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include "def.h"

interface Renderable {
	virtual ~Renderable() = default;
	virtual void render() const noexcept = 0;
};

class Renderer {
	inline static HDC MainDC;
	int windowWidth = 0, windowHeight = 0;

public:
	static void initialize() noexcept;
	explicit Renderer() = default;

	void resize(const int width, const int height) noexcept(false) {
		windowWidth = width;
		windowHeight = height;
	}

	[[nodiscard]] int getWidth() const noexcept { return windowWidth; }
	[[nodiscard]] int getHeight() const noexcept { return windowHeight; }
};

inline static Renderer renderer = Renderer();
