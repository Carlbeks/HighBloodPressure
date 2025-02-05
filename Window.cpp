//
// Created by EmsiaetKadosh on 25-1-14.
//

#include "Window.h"
void Window::render() const noexcept {}
void Window::tick() noexcept {}
void Window::onResize() {}
void FloatWindow::render() const noexcept {
	renderer.fill(20, 20, 40, 40, 0xffeeeeee);
}

void Button::render() const noexcept {}
