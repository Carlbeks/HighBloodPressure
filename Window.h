//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include "def.h"
#include "Renderer.h"
#include "IText.h"

enum class MouseActionCode : char {
	MAC_MOVE, MAC_HOVER, MAC_DOWN, MAC_UP, MAC_DOUBLE
};

class Widget : public IRenderable, public ITickable {
protected:
	int left = 0, top = 0, width = 0, height = 0;

public:
	using Action = Function<void(int)>;
	double x, y, w, h;
	Action hover;// 传入int忽略
	Action longHover;// 传入int表示时间
	Action mouseDown;// 传入int表示变更按键。0左, 1中, 2右
	Action mouseUp;// 传入int表示变更按键。0左, 1中, 2右
	Action mouseLeave;// 传入int忽略
	Action mouseClick;// 传入int表示变更按键。0x0左, 0x1中, 0x2右；0xf表示是否双击
	Color backgroundColor;
	Color foregroundColor;

protected:
	mutable bool hasMouse = false;

public:
	Location location;

protected:
	char unused[6] {};

public:
	unsigned int colorSelector(const Color& clr, const Widget& widget) const {
		return hasMouse ? clr.hover : clr.inactive;
	}

	explicit Widget(const double x, const double y, const double w, const double h, Location location) : x(x), y(y), w(w), h(h), location(location) {}
	virtual void onResize() {}

	virtual bool isMouseIn(int x, int y) noexcept {
		x -= left;
		y -= top;
		return 0 <= x and x <= width and 0 <= y and y <= height;
	}

	virtual void onHover(const int value) noexcept { if (hover) hover(value); }
	virtual void onLongHover(const int value) noexcept { if (longHover) longHover(value); }
	virtual void onMouseDown(const int value) noexcept { if (mouseDown) mouseDown(value); }
	virtual void onMouseUp(const int value) noexcept { if (mouseUp) mouseUp(value); }
	virtual void onMouseLeave(const int value) noexcept { if (mouseLeave) mouseLeave(value); }
	virtual void onMouseClick(const int value) noexcept { if (mouseClick) mouseClick(value); }

	virtual void passEvent(const int action, const int value, const int x, const int y) noexcept {
		if (!isMouseIn(x, y)) {
			if (hasMouse) onMouseLeave(0);
			hasMouse = false;
		}
		hasMouse = true;
		switch (static_cast<char>(action & 0xf)) {
			case static_cast<char>(MouseActionCode::MAC_HOVER):
				onLongHover(0);
				break;
			case static_cast<char>(MouseActionCode::MAC_MOVE):
				onHover(0);
				break;
			case static_cast<char>(MouseActionCode::MAC_DOWN):
				onMouseDown(value);
				break;
			case static_cast<char>(MouseActionCode::MAC_UP):
				onMouseUp(value);
				break;
			case static_cast<char>(MouseActionCode::MAC_DOUBLE):
				onMouseClick(1);
				break;
			default:
				break;
		}
	}
};

class Window : public IRenderable, public ITickable {
protected:
	List<Widget*> widgets;

	Window() = default;

	~Window() override { for (Widget*& widget : widgets) { delete widget; } }

public:
	void render() const noexcept override;
	void tick() noexcept override;
	/**
	 * 在Game.setWindow()时，本窗口开启时调用。
	 * 不应当外部调用。
	 * 如果返回false，则拒绝设置窗口。
	 * @return 是否允许将显示窗口设为自身
	 */
	virtual bool onOpen() = 0;
	/**
	 * 在Game.setWindow()时，本窗口关闭时调用。
	 * 不应当外部调用。
	 * 注意，关闭未必就是删除。
	 */
	virtual void onClose() = 0;
	virtual void onResize();
};

class FloatWindow final : public Window {
public:
	void render() const noexcept override;
	bool onOpen() override { return true; }
	void onClose() override {}
};

class Button : public Widget {
public:
	ObjectHolder<IText> name;
	explicit Button(const double x, const double y, const double w, const double h, const Location location, const ObjectHolder<IText>& text) : Widget(x, y, w, h, location), name(text) {}
	void render() const noexcept override;
	void tick() noexcept override {}
};
