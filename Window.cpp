//
// Created by EmsiaetKadosh on 25-1-14.
//

#include "Window.h"
void Window::render() const noexcept {}
void Window::tick() noexcept {}
void Window::onResize() {}
void FloatWindow::render() const noexcept { // 此处暂时作临时渲染用
	y = 0;
	RenderableString str = L"\\#ffee0000浮窗"_renderable;
	Logger.log(str.toString());
	fontManager.get(1).draw(str, 100, 100, 0xffffffff);
}

void Widget::render() const noexcept {
	renderer.fill(left, top, width, height, colorSelector(backgroundColor, *this));
	RenderableString str = L"123"_renderable;
	Logger.log(str.toString());
	fontManager.get(1).draw(str, 100, 100, 0xffffffff);
}
