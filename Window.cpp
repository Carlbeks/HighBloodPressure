//
// Created by EmsiaetKadosh on 25-1-14.
//

#include "Window.h"

#include "Game.h"
#include "InteractManager.h"

int Window::pop() noexcept {
	game.gc.submit(this);
	Success();
}

void Window::render() const noexcept { for (const ObjectHolder<Widget>& widget : widgets) widget->render(); }
void Window::tick() noexcept { for (ObjectHolder<Widget>& widget : widgets) widget->tick(); }
void Window::onResize() { for (ObjectHolder<Widget>& widget : widgets) widget->onResize(); }
void Window::passEvent(const MouseActionCode action, const MouseButtonCode value, const int x, const int y) noexcept { for (ObjectHolder<Widget>& widget : widgets) widget->passEvent(action, value, x, y); }

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
	ObjectHolder<Widget>& close = widgets.emplace_back(Button(0, 0, interactSettings.actual.captionHeight, interactSettings.actual.captionHeight, Location::RIGHT_TOP, LiteralText(L"\\f\2\u2716")));
	close->absolute();
	close->backgroundColor.hover = 0xffee0000;
	close->backgroundColor.active = 0;
	close->backgroundColor.inactive = 0xffaaaaaa;
	close->backgroundColor.clicked = 0xffee8888;
	close->mouseDown = [](int) { DestroyWindow(MainWindowHandle); };
	close->onTick = [close](int) { if (close->containsMouse()) game.getFloatWindow().push(L"\\#ffee0000关闭窗口"_renderable); };
	close->foregroundColor.hover = 0xff000000;
	close->foregroundColor.active = 0xff000000;
	close->foregroundColor.inactive = 0xff000000;
	close->foregroundColor.clicked = 0xff000000;
	ObjectHolder<Widget>& minmax = widgets.emplace_back(Button(-interactSettings.actual.captionHeight, 0, interactSettings.actual.captionHeight, interactSettings.actual.captionHeight, Location::RIGHT_TOP, LiteralText(L"\\f\2🗖")));
	minmax->absolute();
	minmax->backgroundColor.hover = 0xffcccccc;
	minmax->backgroundColor.active = 0;
	minmax->backgroundColor.inactive = 0xff555555;
	minmax->backgroundColor.clicked = 0xffaaaaaa;
	minmax->foregroundColor.hover = 0xff000000;
	minmax->foregroundColor.active = 0xff000000;
	minmax->foregroundColor.inactive = 0xff000000;
	minmax->foregroundColor.clicked = 0xff000000;
	ObjectHolder<Widget>& hide = widgets.emplace_back(Button(-2 * interactSettings.actual.captionHeight, 0, interactSettings.actual.captionHeight, interactSettings.actual.captionHeight, Location::RIGHT_TOP, LiteralText(L"🗕")));
	hide->absolute();
	hide->backgroundColor.hover = 0xffcccccc;
	hide->backgroundColor.active = 0;
	hide->backgroundColor.inactive = 0xff555555;
	hide->backgroundColor.clicked = 0xffaaaaaa;
	hide->foregroundColor.hover = 0xff000000;
	hide->foregroundColor.active = 0xff000000;
	hide->foregroundColor.inactive = 0xff000000;
	hide->foregroundColor.clicked = 0xff000000;
	ObjectHolder<Widget>& options = widgets.emplace_back(Button(0, 0, interactSettings.actual.captionHeight, interactSettings.actual.captionHeight, Location::LEFT_TOP, LiteralText(L"\\f\2⛭")));
	options->absolute();
	options->backgroundColor.hover = 0xffcccccc;
	options->backgroundColor.active = 0;
	options->backgroundColor.inactive = 0xff555555;
	options->backgroundColor.clicked = 0xffaaaaaa;
	options->foregroundColor.hover = 0xff000000;
	options->foregroundColor.active = 0xff000000;
	options->foregroundColor.inactive = 0xff000000;
	options->foregroundColor.clicked = 0xff000000;
	options->onTick = [options](int) { if (options->containsMouse()) game.getFloatWindow().push(L"\\#ff4488aa设置"_renderable); };
}

bool CaptionWindow::onOpen() { throw InvalidOperationException(L"Should not open CaptionWindow"); }
void CaptionWindow::onClose() { throw InvalidOperationException(L"Should not close CaptionWindow"); }

void CaptionWindow::render() const noexcept {
	renderer.fill(0, 0, renderer.getWidth(), interactSettings.actual.captionHeight, 0xff666666);
	for (const ObjectHolder<Widget>& widget : widgets) widget->render();
}

void CaptionWindow::onResize() {
	int left = 0, right = 0;
	for (ObjectHolder<Widget>& widget : widgets) {
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
	for (const ObjectHolder<RenderableString>& str : strings.get()) {
		height += str->getHeight();
		if (str->getWidth() > width) width = str->getWidth();
	}
	width += interactSettings.actual.floatWindowMargin * 2;
	height += interactSettings.actual.floatWindowMargin * 2;
	if (x + width > renderer.getWidth()) x = renderer.getWidth() - width;
	y = y < height ? 0 : y - height;
	renderer.fill(x, y, width, height, interactSettings.constants.floatWindowBackground);

	const int xf = x + interactSettings.actual.floatWindowMargin;
	int yf = y + interactSettings.actual.floatWindowMargin;

	for (const ObjectHolder<RenderableString>& str : strings.get()) {
		fontManager.get(1).draw(*str, xf, yf);
		yf += str->getHeight();
	}

	strings.async();
}


unsigned int Widget::colorSelector(const Color& clr) const {
	if (!isActive) return clr.inactive;
	if (!hasMouse) return clr.active;
	if (interactManager.getKey(VK_LBUTTON).isPressed() || interactManager.getKey(VK_RBUTTON).isPressed() || interactManager.getKey(VK_MBUTTON).isPressed()) return clr.clicked;
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
		width = renderer.getWidth() * w;
		height = renderer.getHeight() * h;
		switch (location) {
			case Location::LEFT_TOP:
				left = renderer.getWidth() * x;
				top = renderer.getHeight() * y;
				break;
			case Location::LEFT:
				left = renderer.getWidth() * x;
				top = static_cast<int>(renderer.getHeight() * y) + (renderer.getHeight() - height >> 1);
				break;
			case Location::LEFT_BOTTOM:
				left = renderer.getWidth() * x;
				top = static_cast<int>(renderer.getHeight() * y) + renderer.getHeight() - height;
				break;
			case Location::TOP:
				left = static_cast<int>(renderer.getWidth() * x) + (renderer.getWidth() - width >> 1);
				top = renderer.getHeight() * y;
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
				top = renderer.getHeight() * y;
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

ConfirmWindow& ConfirmWindow::requireCancel(const Function<void(Button&)>& func) {
	cancel = dynamic_cast<Button*>(widgets.emplace_back(std::move(Button(0, 0.1, 0.4, 0.08, Location::CENTER, LiteralText(L"\\#ff44ee66Cancel")))).ptr());
	cancel->mouseUp = [this](int) {
		game.tasks.pushNewed(allocatedFor(new Task([this](Task& self) {
			if (game.closeWindow(this)) this->onClose();
			self.pop();
		})));
	};
	if (func) func(*cancel);
	return *this;
}

void ConfirmWindow::onClose() { pop(); }
