//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include <thread>

#include "def.h"
#include "exception.h"
#include "Task.h"

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
	friend class Game;
	friend class Font;
	mutable List<HGDIOBJ> failed;
	inline static BLENDFUNCTION blendFunction = {
		.BlendOp = AC_SRC_OVER, // Only
		.BlendFlags = 0, // Must 0
		.SourceConstantAlpha = 255, // 预乘
		.AlphaFormat = 0, // Not AC_SRC_ALPHA
	};
	HDC MainDC = nullptr; // 8
	HDC resizeCopyDC = nullptr; // 8
	HBITMAP resizeCopyBitmap = nullptr; // 8
	HDC canvasDC = nullptr; // 8
	HBITMAP canvasBitmap = nullptr; // 8
	HDC assistDC = nullptr; // 8
	HBITMAP assistBitmap = nullptr; // 8
	std::thread::id renderThread = std::this_thread::get_id();
	int windowWidth = 0, windowHeight = 0; // 4 + 4
	/**
	 * 指示实时大小。为了防抖，只会在改变窗口大小结束时resize并重写windowWidth和windowHeight
	 */
	int syncWidth = 0, syncHeight = 0; // 4 + 4
	/**
	 * 缓存resizeCopyBitmap的宽高
	 */
	int resizeCopyWidth = 0, resizeCopyHeight = 0; // 4 + 4
	bool isRendering = false; // 1
	bool isResizing = false; // 1

public:
	byte windowSize = 0; // 1, Windows: SIZE_***
	byte reserved[5]{};
	Task resizeReloadBitmap{ [this](Task& task) {
		Logger.info(L"Scheduled task: resize reload bitmap " + std::to_wstring(windowWidth) + L" * " + std::to_wstring(windowHeight));
		if (!canvasBitmap) {
			canvasBitmap = CreateCompatibleBitmap(MainDC, windowWidth, windowHeight);
			if (canvasBitmap) SelectObject(canvasDC, canvasBitmap);
		}
		if (!assistBitmap) {
			assistBitmap = CreateCompatibleBitmap(canvasDC, windowWidth, windowHeight);
			if (assistBitmap) SelectObject(assistDC, assistBitmap);
		}
		if (canvasBitmap && assistBitmap) {
			task.pop();
			Logger.info(L"Successfully reload bitmap " + ptrtow(reinterpret_cast<QWORD>(canvasBitmap)) + L" " + ptrtow(reinterpret_cast<QWORD>(assistBitmap)));
			this->resizeEnd();
		}
	} };

private:
	void gameStartRender() noexcept;
	void gameEndRender() noexcept;

public:
	Renderer() { Logger.put(L"Renderer created"); }

	~Renderer() override {
		Logger.put(L"Renderer destroyed");
		if (assistDC) DeleteDC(assistDC);
		if (canvasDC) DeleteDC(canvasDC);
		if (resizeCopyDC) DeleteDC(resizeCopyDC);
		if (canvasBitmap) DeleteObject(canvasBitmap);
		if (assistBitmap) DeleteObject(assistBitmap);
		if (resizeCopyBitmap) DeleteObject(resizeCopyBitmap);
	}

	void initialize() noexcept;
	/**
	 * 负责转发所有resize信息
	 */
	void resize(int width, int height) noexcept(false);
	void syncSize(int width, int height) noexcept(false);
	[[nodiscard]] int getWidth() const noexcept { return windowWidth; }
	[[nodiscard]] int getHeight() const noexcept { return windowHeight; }
	[[nodiscard]] int getSyncWidth() const noexcept { return syncWidth; }
	[[nodiscard]] int getSyncHeight() const noexcept { return syncHeight; }
	[[nodiscard]] bool checkResizing() const noexcept { return isResizing; }
	void tick() noexcept override {}

	/**
	 * @attention 会忽略A透明度值
	 * @param argb ARGB式颜色
	 * @return int BGR式颜色
	 */
	[[nodiscard]] static unsigned int changeColorFormat(const unsigned int argb) { return argb << 16 & 0xff0000 | argb & 0xff00 | argb >> 16 & 0xff; }

	void assertRendering() const noexcept(false) { if (!isRendering) throw InvalidOperationException(L"Operation should be done while rendering"); }

	void assertRenderThread() const noexcept(false) { if (std::this_thread::get_id() != renderThread) throw InvalidOperationException(L"Operation should be done in render thread"); }

	void resizeStart() noexcept { isResizing = true; }

	void resizeShow() const noexcept { StretchBlt(MainDC, 0, 0, syncWidth, syncHeight, resizeCopyDC, 0, 0, resizeCopyWidth, resizeCopyHeight, SRCCOPY); }

	void resizeEnd() noexcept {
		isResizing = false;
		deleteObject(resizeCopyBitmap);
		resizeCopyBitmap = nullptr;
		resizeCopyWidth = 0;
		resizeCopyHeight = 0;
	}

	void deleteObject(HGDIOBJ obj) const noexcept {
		for (List<HGDIOBJ>::const_iterator iter = failed.cbegin(); iter != failed.cend(); ++iter)
			if (!DeleteObject(*iter)) Logger.error(L"DeleteObject failed again. Deleting: " + std::to_wstring(reinterpret_cast<QWORD>(*iter)));
			else failed.erase(iter);
		if (obj && !DeleteObject(obj)) {
			failed.push_back(obj);
			Logger.error(L"DeleteObject failed. Deleting: " + std::to_wstring(reinterpret_cast<QWORD>(obj)));
		}
	}

	void fill(const int x, const int y, const int w, const int h, const unsigned int color) const {
		assertRendering();
		//assertRenderThread();
		if ((color & 0xff000000) == 0) return;
		if ((color & 0xff000000) == 0xff000000) {
			const RECT rect{
				.left = x,
				.top = y,
				.right = x + w,
				.bottom = y + h
			};
			const HBRUSH clr = CreateSolidBrush(changeColorFormat(color));
			FillRect(canvasDC, &rect, clr);
			deleteObject(clr);
		} else {
			const RECT rect{
				.left = 0,
				.top = 0,
				.right = x + w > windowWidth ? windowWidth - x : w,
				.bottom = y + h > windowHeight ? windowHeight - y : h
			};
			blendFunction.SourceConstantAlpha = color >> 24;
			const HBRUSH clr = CreateSolidBrush(changeColorFormat(color));
			FillRect(assistDC, &rect, clr);
			deleteObject(clr);
			if (!AlphaBlend(canvasDC, x, y, rect.right, rect.bottom, assistDC, 0, 0, rect.right, rect.bottom, blendFunction)) Logger.error(L"AlphaBlend failed");
		}
	}

	void fill(const RECT* const rect, const unsigned int color) const {
		assertRendering();
		//assertRenderThread();
		if ((color & 0xff000000) == 0) return;
		if ((color & 0xff000000) == 0xff000000) { const HBRUSH clr = CreateSolidBrush(changeColorFormat(color));
			FillRect(canvasDC, rect, clr);
			deleteObject(clr);
		} else {
			const RECT r{
				.left = 0,
				.top = 0,
				.right = rect->right > windowWidth ? windowWidth : rect->right - rect->left,
				.bottom = rect->bottom > windowHeight ? windowHeight : rect->bottom - rect->top
			};
			blendFunction.SourceConstantAlpha = color >> 24;
			const HBRUSH clr = CreateSolidBrush(changeColorFormat(color));
			FillRect(assistDC, &r, clr);
			deleteObject(clr);
			if (!AlphaBlend(canvasDC, rect->left, rect->top, r.right, r.bottom, assistDC, 0, 0, r.right, r.bottom, blendFunction)) Logger.error(L"AlphaBlend failed");
		}
	}
};

extern Renderer renderer;
