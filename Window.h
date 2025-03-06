//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include "def.h"
#include "gc.h"
#include "Renderer.h"
#include "IText.h"

class WindowManager;

enum class MouseActionCode : int {
	MAC_MOVE = 0,
	MAC_HOVER = 1,
	MAC_DOWN = 2,
	MAC_UP = 3,
	MAC_DOUBLE = 4,
	MAC_LEAVE = 5
};

enum class MouseButtonCodeEnum : int {
	MBC_L_DOWN = 0x1,
	MBC_R_DOWN = 0x2,
	MBC_M_DOWN = 0x4,
	MBC_L_CHANGE = 0x10,
	MBC_R_CHANGE = 0x20,
	MBC_M_CHANGE = 0x40,
};

/**
 * @see MouseButtonCodeEnum
 */
using MouseButtonCode = int;

class Widget : public IRenderable, public ITickable {
protected:
	int left = 0, top = 0, width = 0, height = 0;

public:
	using Action = Function<void(int)>;
	double x, y, w, h;
	Action mouseHover; // 传入int为：0，鼠标移动；其余，鼠标在其上的长时间悬浮
	Action mouseDown; // 传入int表示变更按键。0左, 1中, 2右
	Action mouseUp; // 传入int表示变更按键。0左, 1中, 2右
	Action mouseLeave; // 传入int忽略
	Action mouseClick; // 传入int表示变更按键。0x0左, 0x1中, 0x2右；0xf表示是否双击
	Action onTick; // 传入int忽略
	Color backgroundColor;
	Color foregroundColor{ TextColor };

protected:
	mutable bool hasMouse = false;
	mutable bool isActive = true;
	bool isAbsoluteLocation = false;

public:
	Location location;
	Location textLocation = Location::CENTER; // 多余字节预声明备用

protected:
	char unused[3]{};

public:
	explicit Widget(const double x, const double y, const double w, const double h, const Location location) : x(x), y(y), w(w), h(h), location(location) {}

	unsigned int colorSelector(const Color& clr) const;
	void render() const noexcept override;

	Widget& alignLocation(const Location loc) noexcept {
		location = loc;
		return *this;
	}

	Widget& alignTextLocation(const Location loc) noexcept {
		textLocation = loc;
		return *this;
	}

	Widget& absolute(const bool value = true) {
		if (value == isAbsoluteLocation) return *this;
		isAbsoluteLocation = value;
		if (renderer.getWidth() != 0 && renderer.getHeight() != 0) onResize();
		return *this;
	}

	bool containsMouse() const noexcept { return hasMouse; }

	virtual bool isMouseIn(int x, int y) noexcept {
		x -= left;
		y -= top;
		return 0 <= x and x <= width and 0 <= y and y <= height;
	}

	virtual void onResize();
	virtual void onHover(const int value) noexcept { if (mouseHover) mouseHover(value); }
	virtual void onMouseDown(const MouseButtonCode code) noexcept { if (mouseDown) mouseDown(code); }
	virtual void onMouseUp(const MouseButtonCode code) noexcept { if (mouseUp) mouseUp(code); }
	virtual void onMouseLeave(const int value) noexcept { if (mouseLeave) mouseLeave(value); }
	virtual void onMouseClick(const int value) noexcept { if (mouseClick) mouseClick(value); }
	void tick() noexcept override { if (onTick) onTick(0); }

	virtual void passEvent(const MouseActionCode action, const MouseButtonCode value, const int x, const int y) noexcept {
		if (action == MouseActionCode::MAC_LEAVE || !isMouseIn(x, y)) {
			if (hasMouse) onMouseLeave(0);
			hasMouse = false;
			return;
		}
		hasMouse = true;
		switch (action) {
			case MouseActionCode::MAC_HOVER:
				onHover(1);
				break;
			case MouseActionCode::MAC_MOVE:
				onHover(0);
				break;
			case MouseActionCode::MAC_DOWN:
				onMouseDown(value);
				break;
			case MouseActionCode::MAC_UP:
				onMouseUp(value);
				break;
			case MouseActionCode::MAC_DOUBLE:
				onMouseClick(1);
				break;
			default:
				break;
		}
	}
};

class Window : public AnywhereEditable<Window, WindowManager>, public IRenderable, public ITickable {
protected:
	friend class Garbage<Window>;
	friend class AnywhereEditableList<Window, WindowManager>;
	List<ObjectHolder<Widget>> widgets;

	Window() = default;

	~Window() override {}

public:
	int pop() noexcept override;
	void render() const noexcept override;
	void tick() noexcept override;
	/**
	 * 在Game.setWindow()时，本窗口开启时调用。
	 * 不应当外部调用。
	 * 如果返回false，则拒绝设置窗口。
	 * @return 是否允许将显示窗口设为自身
	 */
	virtual bool onOpen() { return true; }
	/**
	 * 在Game.setWindow()时，本窗口关闭时调用。
	 * 不应当外部调用。
	 * 注意，关闭未必就是删除。
	 */
	virtual void onClose() = 0;
	virtual void onResize();
	virtual void passEvent(MouseActionCode action, MouseButtonCode value, int x, int y) noexcept;
};

class WindowManager final : public AnywhereEditableList<Window, WindowManager>, public IRenderable, public ITickable {
public:
	void render() const noexcept override { for (Window& i : *this) i.render(); }
	void tick() noexcept override { for (Window& i : *this) i.tick(); }
	int pop(Window* value) noexcept override;
	void clear() noexcept;
	void onResize() noexcept { for (Window& i : *this) i.onResize(); }
};

class CaptionWindow final : public Window {
public:
	explicit CaptionWindow();
	bool onOpen() override;
	void onClose() override;
	void render() const noexcept override;
	void onResize() override;
};

class FloatWindow final : public Window {
	mutable int x = 0; // 标记渲染起始点
	mutable int y = 0; // 标记渲染起始点
	typedef List<ObjectHolder<RenderableString>> lt;
	SynchronizedHolder<lt> strings{};

public:
	explicit FloatWindow() : Window() {
		strings.setNew<lt>(std::move(lt()));
		strings.ok();
		strings.async();
	}

	void clear() { strings.setNew<lt>(std::move(lt())); }

	void push(const ObjectHolder<RenderableString>& string) const { if (strings.ptrNew()) strings.getNew().push_back(string); }
	void push(ObjectHolder<RenderableString>&& string) const { if (strings.ptrNew()) strings.getNew().push_back(std::move(string)); }
	void render() const noexcept override;
	void tick() noexcept override {}
	bool onOpen() override { return true; }
	void onClose() override {}
	void update() const noexcept { strings.ok(); }
};

class Button : public Widget {
public:
	ObjectHolder<IText> name;
	explicit Button(const double x, const double y, const double w, const double h, const Location location, const ObjectHolder<IText>& text) : Widget(x, y, w, h, location), name(text) {}
	explicit Button(const double x, const double y, const double w, const double h, const Location location, ObjectHolder<IText>&& text) : Widget(x, y, w, h, location), name(std::move(text)) {}
	void render() const noexcept override;
};


class ConfirmWindow : public Window {
public:
	ObjectHolder<IText> text;
	Button
		*confirm = nullptr,
		*cancel = nullptr;

private:
	explicit ConfirmWindow(const ObjectHolder<IText>& text) : Window(), text(text) {}
	explicit ConfirmWindow(ObjectHolder<IText>&& text) : Window(), text(std::move(text)) {}

public:
	~ConfirmWindow() override {
		if (confirm) delete confirm;
		confirm = nullptr;
		if (cancel) delete cancel;
		cancel = nullptr;
	}

	void render() const noexcept override {}

	ConfirmWindow& requireConfirm(const Function<void(Button&)>& func = {}) {
		confirm = dynamic_cast<Button*>(widgets.emplace_back(std::move(Button(0, 0, 0.4, 0.08, Location::CENTER, LiteralText(L"\\#ff44ee66Confirm")))).ptr());
		if (func) func(*confirm);
		return *this;
	}

	ConfirmWindow& requireCancel(const Function<void(Button&)>& func = {});

	ConfirmWindow&& move() noexcept { return std::move(*this); }

	void onClose() override;

	static ConfirmWindow* create(const ObjectHolder<IText>& text) { return new ConfirmWindow(text); }
	static ConfirmWindow* create(ObjectHolder<IText>&& text) { return new ConfirmWindow(std::move(text)); }
};
