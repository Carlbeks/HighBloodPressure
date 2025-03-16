//
// Created by EmsiaetKadosh on 25-1-14.
//

#include "Window.h"

#include "Animation.h"
#include "Game.h"
#include "InteractManager.h"

int Window::pop() noexcept {
	gc.submit(this);
	Success();
}

void Window::render() const noexcept { for (const Widget* widget : widgets) widget->render(); }
void Window::tick() noexcept { for (Widget* widget : widgets) widget->tick(); }
void Window::onResize() { for (Widget* widget : widgets) widget->onResize(); }
void Window::passEvent(const MouseActionCode action, const MouseButtonCode value, const int x, const int y) noexcept { for (Widget* widget : widgets) widget->passEvent(action, value, x, y); }

int WindowManager::pop(Window* value) noexcept {
	if (value->list != static_cast<AnywhereEditableList*>(this)) {
		Logger.error(L"AnywhereEditableList::pop() : value is not in this list");
		Failed();
	}
	value->list = nullptr;
	value->next->prev = value->prev;
	value->prev->next = value->next;
	value->onClose();
	Success();
}

void WindowManager::clear() noexcept {
	for (Window& window : *this) {
		window.list = nullptr;
		window.onClose();
	}
	head.next = nullptr;
	tail.prev = nullptr;
}

CaptionWindow::CaptionWindow() {
	Widget* close = widgets.emplace_back(Button(0, 0, interactSettings.actual.captionHeight, interactSettings.actual.captionHeight, Location::RIGHT_TOP, LiteralText(L"\\f\1\u2716")));
	close->mouseClick = [](Widget&, MouseButtonCode) { DestroyWindow(MainWindowHandle); };
	close->onTick = [this](const Widget& self, MouseButtonCode) { if (self.containsMouse()) game.getFloatWindow().push(L"\\#ffee0000关闭窗口"_renderable); };
	close->absolute();
	close->backgroundColor.hover = 0xffee0000;
	close->backgroundColor.active = 0;
	close->backgroundColor.inactive = 0xffaaaaaa;
	close->backgroundColor.clicked = 0xffee8888;
	close->foregroundColor.hover = 0xff000000;
	close->foregroundColor.active = 0xff000000;
	close->foregroundColor.inactive = 0xff000000;
	close->foregroundColor.clicked = 0xff000000;

	Widget* minmax = widgets.emplace_back(Button(-interactSettings.actual.captionHeight, 0, interactSettings.actual.captionHeight, interactSettings.actual.captionHeight, Location::RIGHT_TOP, LiteralText(L"\\f\1🗖")));
	minmax->absolute();
	minmax->backgroundColor.hover = 0xffcccccc;
	minmax->backgroundColor.active = 0;
	minmax->backgroundColor.inactive = 0xff555555;
	minmax->backgroundColor.clicked = 0xffaaaaaa;
	minmax->foregroundColor.hover = 0xff000000;
	minmax->foregroundColor.active = 0xff000000;
	minmax->foregroundColor.inactive = 0xff000000;
	minmax->foregroundColor.clicked = 0xff000000;

	Widget* hide = widgets.emplace_back(Button(-2 * interactSettings.actual.captionHeight, 0, interactSettings.actual.captionHeight, interactSettings.actual.captionHeight, Location::RIGHT_TOP, LiteralText(L"\\f\1🗕")));
	hide->mouseClick = [](Widget&, MouseButtonCode) { ShowWindow(MainWindowHandle, SW_MINIMIZE); };
	hide->absolute();
	hide->backgroundColor.hover = 0xffcccccc;
	hide->backgroundColor.active = 0;
	hide->backgroundColor.inactive = 0xff555555;
	hide->backgroundColor.clicked = 0xffaaaaaa;
	hide->foregroundColor.hover = 0xff000000;
	hide->foregroundColor.active = 0xff000000;
	hide->foregroundColor.inactive = 0xff000000;
	hide->foregroundColor.clicked = 0xff000000;

	Widget* options = widgets.emplace_back(Button(0, 0, interactSettings.actual.captionHeight, interactSettings.actual.captionHeight, Location::LEFT_TOP, LiteralText(L"\\f\1⛭")));
	options->onTick = [](const Widget& self, MouseButtonCode) {
		if (self.containsMouse()) {
			game.getFloatWindow().push(L"\\#ff4488aa设置"_renderable);
			game.getFloatWindow().push(L"\\.ff4488aa\\#ff000000右键以刷新窗口绘制"_renderable);
		}
	};
	options->mouseClick = [](Widget&, const MouseButtonCode code) { if (static_cast<int>(MouseButtonCodeEnum::MBC_R_DOWN) & code) { game.tasks.pushThis(renderer.resizeReloadBitmap); } };
	options->absolute();
	options->backgroundColor.hover = 0xffcccccc;
	options->backgroundColor.active = 0;
	options->backgroundColor.inactive = 0xff555555;
	options->backgroundColor.clicked = 0xffaaaaaa;
	options->foregroundColor.hover = 0xff000000;
	options->foregroundColor.active = 0xff000000;
	options->foregroundColor.inactive = 0xff000000;
	options->foregroundColor.clicked = 0xff000000;
}

bool CaptionWindow::onOpen() { throw InvalidOperationException(L"Should not open CaptionWindow"); }
void CaptionWindow::onClose() { throw InvalidOperationException(L"Should not close CaptionWindow"); }

void CaptionWindow::render() const noexcept {
	renderer.fill(0, 0, renderer.getWidth(), interactSettings.actual.captionHeight, 0xff666666);
	for (const Widget* widget : widgets) widget->render();
}

void CaptionWindow::onResize() {
	int left = 0, right = 0;
	for (Widget* widget : widgets) {
		widget->w = interactSettings.actual.captionHeight;
		widget->h = interactSettings.actual.captionHeight;
		switch (widget->location) {
			case Location::LEFT_TOP:
				widget->x = left;
				left += interactSettings.actual.captionHeight;
				break;
			case Location::RIGHT_TOP:
				widget->x = right;
				right -= interactSettings.actual.captionHeight;
				break;
			default:
				break;
		}
	}
	Window::onResize();
}

void FloatWindow::render() const noexcept {
	if (not interactManager.isInWindow()) {
		strings.async();
		return;
	}
	if (strings.get().empty()) {
		strings.async();
		return;
	}
	x = interactManager.getMouseX();
	y = interactManager.getMouseY();
	int height = 0, width = 0;
	for (const RenderableString* str : strings.get()) {
		height += str->getHeight();
		if (str->getWidth() > width) width = str->getWidth();
	}
	const int fwm2 = interactSettings.actual.floatWindowMargin * 2;
	x += fwm2; // 做一个偏移。不让小窗左下角直接对准鼠标
	width += fwm2;
	height += fwm2;
	if (x + width > renderer.getWidth()) x = renderer.getWidth() - width;
	y = y < height ? 0 : y - height;
	renderer.fill(x, y, width, height, interactSettings.constants.floatWindowBackground);

	const int xf = x + interactSettings.actual.floatWindowMargin;
	int yf = y + interactSettings.actual.floatWindowMargin;

	for (const RenderableString* str : strings.get()) {
		fontManager.getDefault().draw(*str, xf, yf);
		yf += str->getHeight();
	}

	strings.async();
}


unsigned int Widget::colorSelector(const Color& clr) const {
	if (!isActive) return clr.inactive;
	if (!hasMouse) return clr.active;
	if (hasMouseTrigger && (interactManager.getKey(VK_LBUTTON).isPressed() || interactManager.getKey(VK_RBUTTON).isPressed() || interactManager.getKey(VK_MBUTTON).isPressed())) return clr.clicked;
	return clr.hover;
}

void Widget::render() const noexcept { renderer.fill(left, top, width, height, colorSelector(backgroundColor)); }

void Widget::onResize() {
	if (isAbsoluteLocation) {
		width = static_cast<int>(w);
		height = static_cast<int>(h);
		switch (location) {
			case Location::LEFT_TOP:
				left = static_cast<int>(x);
				top = static_cast<int>(y);
				break;
			case Location::LEFT:
				left = static_cast<int>(x);
				top = static_cast<int>(y) + (renderer.getHeight() - height >> 1);
				break;
			case Location::LEFT_BOTTOM:
				left = static_cast<int>(x);
				top = static_cast<int>(y) + renderer.getHeight() - height;
				break;
			case Location::TOP:
				left = static_cast<int>(x) + (renderer.getWidth() - width >> 1);
				top = static_cast<int>(y);
				break;
			case Location::CENTER:
				left = static_cast<int>(x) + (renderer.getWidth() - width >> 1);
				top = static_cast<int>(y) + (renderer.getHeight() - height >> 1);
				break;
			case Location::BOTTOM:
				left = static_cast<int>(x) + (renderer.getWidth() - width >> 1);
				top = static_cast<int>(y) + renderer.getHeight() - height;
				break;
			case Location::RIGHT_TOP:
				left = static_cast<int>(x) + renderer.getWidth() - width;
				top = static_cast<int>(y);
				break;
			case Location::RIGHT:
				left = static_cast<int>(x) + renderer.getWidth() - width;
				top = static_cast<int>(y) + (renderer.getHeight() - height >> 1);
				break;
			case Location::RIGHT_BOTTOM:
				left = static_cast<int>(x) + renderer.getWidth() - width;
				top = static_cast<int>(y) + renderer.getHeight() - height;
				break;
		}
	} else {
		width = static_cast<int>(renderer.getWidth() * w);
		height = static_cast<int>(renderer.getHeight() * h);
		switch (location) {
			case Location::LEFT_TOP:
				left = static_cast<int>(renderer.getWidth() * x);
				top = static_cast<int>(renderer.getHeight() * y);
				break;
			case Location::LEFT:
				left = static_cast<int>(renderer.getWidth() * x);
				top = static_cast<int>(renderer.getHeight() * y) + (renderer.getHeight() - height >> 1);
				break;
			case Location::LEFT_BOTTOM:
				left = static_cast<int>(renderer.getWidth() * x);
				top = static_cast<int>(renderer.getHeight() * y) + renderer.getHeight() - height;
				break;
			case Location::TOP:
				left = static_cast<int>(renderer.getWidth() * x) + (renderer.getWidth() - width >> 1);
				top = static_cast<int>(renderer.getHeight() * y);
				break;
			case Location::CENTER:
				left = static_cast<int>(renderer.getWidth() * x) + (renderer.getWidth() - width >> 1);
				top = static_cast<int>(renderer.getHeight() * y) + (renderer.getHeight() - height >> 1);
				break;
			case Location::BOTTOM:
				left = static_cast<int>(renderer.getWidth() * x) + (renderer.getWidth() - width >> 1);
				top = static_cast<int>(renderer.getHeight() * y) + renderer.getHeight() - height;
				break;
			case Location::RIGHT_TOP:
				left = static_cast<int>(renderer.getWidth() * x) + renderer.getWidth() - width;
				top = static_cast<int>(renderer.getHeight() * y);
				break;
			case Location::RIGHT:
				left = static_cast<int>(renderer.getWidth() * x) + renderer.getWidth() - width;
				top = static_cast<int>(renderer.getHeight() * y) + (renderer.getHeight() - height >> 1);
				break;
			case Location::RIGHT_BOTTOM:
				left = static_cast<int>(renderer.getWidth() * x) + renderer.getWidth() - width;
				top = static_cast<int>(renderer.getHeight() * y) + renderer.getHeight() - height;
				break;
		}
	}
}

void Button::render() const noexcept {
	Widget::render();
	if (name) fontManager.getDefault().drawCenter(name->getRenderableString(), left, top, width, height, colorSelector(foregroundColor));
}

ConfirmWindow& ConfirmWindow::requireConfirm(const Function<void(Button&)>& func) {
	confirm = dynamic_cast<Button*>(widgets.emplace_back(std::move(Button(0, 0, 0.4, 0.08, Location::CENTER, LiteralText(L"Confirm")))).ptr());
	confirm->location = Location::CENTER;
	confirm->backgroundColor.active = 0x99000000;
	confirm->backgroundColor.hover = 0x9900ff00;
	confirm->backgroundColor.clicked = 0xff00ee00;
	confirm->foregroundColor.active = 0xff00ee00;
	confirm->foregroundColor.hover = 0xff000000;
	confirm->foregroundColor.clicked = 0xff000000;
	confirm->y = 0.21;
	confirm->h = 0.08;
	if (cancel) {
		confirm->w = 0.25;
		confirm->x = -0.125;
		cancel->x = 0.125;
		cancel->w = 0.25;
		cancel->onResize();
	} else {
		confirm->w = 0.5;
		confirm->x = 0;
	}
	confirm->onTick = [](Widget& confirm, MouseButtonCode) { if (confirm.containsMouse()) confirm.backgroundColor.hover = static_cast<Button&>(confirm).animation.adaptsColor(0x99008800, 0x9900ff00); };
	confirm->mouseLeave = [](Widget& confirm, MouseButtonCode) { static_cast<Button&>(confirm).animation.reset(); };
	if (func) func(*confirm);
	confirm->onResize();
	return *this;
}

ConfirmWindow& ConfirmWindow::requireCancel(const Function<void(Button&)>& func) {
	cancel = dynamic_cast<Button*>(widgets.emplace_back(std::move(Button(0, 0.1, 0.4, 0.08, Location::CENTER, LiteralText(L"Cancel")))).ptr());
	cancel->mouseClick = [this](Widget&, MouseButtonCode) {
		game.tasks.pushNewed(allocatedFor(new Task([this](Task& self) {
			if (game.closeWindow(this)) this->onClose();
			self.pop();
			})));
	};
	cancel->location = Location::CENTER;
	cancel->backgroundColor.active = 0x99000000;
	cancel->backgroundColor.hover = 0x99ff0000;
	cancel->backgroundColor.clicked = 0xffee0000;
	cancel->foregroundColor.active = 0xffee0000;
	cancel->foregroundColor.hover = 0xff000000;
	cancel->foregroundColor.clicked = 0xff000000;
	cancel->y = 0.21;
	cancel->h = 0.08;
	if (confirm) {
		cancel->x = 0;
		cancel->w = 0.25;
		confirm->x = -0.125;
		confirm->w = 0.25;
		confirm->onResize();
	} else {
		cancel->x = 0.125;
		cancel->w = 0.5;
	}
	cancel->onTick = [](Widget& cancel, MouseButtonCode) { if (cancel.containsMouse()) cancel.backgroundColor.hover = static_cast<Button&>(cancel).animation.adaptsColor(0x99880000, 0x99ff0000); };
	cancel->mouseLeave = [](Widget& cancel, int) { static_cast<Button&>(cancel).animation.reset(); };
	if (func) func(*cancel);
	cancel->onResize();
	return *this;
}

void ConfirmWindow::onClose() { pop(); }
