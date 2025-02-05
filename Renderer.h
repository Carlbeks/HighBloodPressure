//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include <thread>

#include "def.h"
#include "exception.h"

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

struct Color {
	unsigned int inactive = 0xff777777;
	unsigned int active = 0xff000000;
	unsigned int hover = 0xff444444;
	unsigned int clicked = 0xffeeeeee;
};

inline static constexpr Color TextColor = {
	.inactive = 0xff333333,
	.active = 0xffeeeeee,
	.hover = 0xffeeeeee,
	.clicked = 0xff000000
};

class Renderer final : public ITickable {
	/**
	 * 后续可能用不到，可能可删
	 */
	Game* pGame = nullptr;

	friend class Game;
	inline static HDC MainDC;
	inline static HDC assistDC;
	inline static BLENDFUNCTION blendFunction = {
		.BlendOp = AC_SRC_OVER,// Only
		.BlendFlags = 0,// Must 0
		.SourceConstantAlpha = 255,// 预乘
		.AlphaFormat = AC_SRC_ALPHA,// Only
	};
	std::thread::id renderThread = std::this_thread::get_id();
	int windowWidth = 0, windowHeight = 0;
	int resizeTime = 0;
	bool isRendering = false;

	void gameStartRender() noexcept {
		isRendering = true;
		renderThread = std::this_thread::get_id();
	}

	void gameEndRender() noexcept { isRendering = false; }

public:
	static void initialize() noexcept;
	explicit Renderer() { Logger.put(L"Renderer created"); }
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

	/**
	 * @attention 会忽略A透明度值
	 * @param argb ARGB式颜色
	 * @return int BGR式颜色
	 */
	[[nodiscard]] static unsigned int changeColorFormat(const unsigned int argb) { return (argb & 0xff) << 16 | (argb & 0xff00) | (argb & 0xff0000) >> 16; }

	void assertRendering() const noexcept(false) { if (!isRendering) throw InvalidOperationException(L"Operation should be done while rendering"); }

	void assertRenderThread() const noexcept(false) { if (std::this_thread::get_id() != renderThread) throw InvalidOperationException(L"Operation should be done in render thread"); }

	void fill(const int x, const int y, const int w, const int h, const unsigned int color) const {
		assertRendering();
		//assertRenderThread();
		if (color & 0xff000000 == 0) return;
		const RECT rect{
			.left = x,
			.top = y,
			.right = x + w,
			.bottom = y + h
		};
		HBRUSH clr = CreateSolidBrush(changeColorFormat(color));
		if ((color & 0xff000000) == 0xff000000) FillRect(MainDC, &rect, clr);
		else {
			FillRect(assistDC, &rect, clr);
			AlphaBlend(MainDC, x, y, w, h, assistDC, x, y, w, h, blendFunction);
		}
		DeleteObject(clr);
	}
};

extern Renderer renderer;
