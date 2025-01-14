//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include "Renderer.h"

class Window : public Renderable {
protected:
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
};
