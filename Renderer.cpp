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
	if (game.getWindow()) game.getWindow()->onResize();
}
