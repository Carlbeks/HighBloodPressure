//
// Created by EmsiaetKadosh on 25-1-14.
//

#include "Renderer.h"

#include "Game.h"
#include "hbp.h"

void Renderer::initialize() noexcept { MainDC = GetDC(MainWindowHandle); }

void Renderer::resize(const int width, const int height) noexcept(false) {
	windowWidth = width;
	windowHeight = height;
	if (resizeTime > 0) return; // 防抖、防止频繁触发
	if (resizeTime < 0) {
		resizeTime = 20;
		return;
	}
	if (game.getWindow()) game.getWindow()->onResize();
}
