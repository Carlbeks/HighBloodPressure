//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include "def.h"
#include "Renderer.h"

class Widget : public Renderable {
protected:
	int left = 0, top = 0, width = 0, height = 0;
	mutable bool hasMouse = false;

public:
	using Action = Function<void()>;
	double x, y, w, h;
	Action hover;
	Action longHover;
	Action mouseDown;
	Action mouseUp;
	Action mouseLeave;
	explicit Widget(const double x, const double y, const double w, const double h) : x(x), y(y), w(w), h(h) {}
	void render() const noexcept override {}
	virtual void onResize() {}

	virtual bool isMouseIn(int x, int y) noexcept {
		x -= left;
		y -= top;
		if (0 <= x and x <= width and 0 <= y and y <= height) hasMouse = true;
		hasMouse = false;
		return hasMouse;
	}

	virtual void onHover() noexcept { if (hover) hover(); }
	virtual void onLongHover() noexcept { if (longHover) longHover(); }
	virtual void onMouseDown() noexcept { if (mouseDown) mouseDown(); }
	virtual void onMouseUp() noexcept { if (mouseUp) mouseUp(); }
	virtual void onMouseLeave() noexcept { if (mouseLeave) mouseLeave(); }
};

class Window : public Renderable {
protected:
	List<Widget*> widgets;

	Window() = default;

public:
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

class Button final : public Widget {
public:
	explicit Button(const double x, const double y, const double w, const double h) : Widget(x, y, w, h) {}
};
