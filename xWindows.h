//
// Created by EmsiaetKadosh on 25-1-18.
//

#pragma once

#include "Window.h"

class StartWindow final : public Window {
	TranslatableText title = TranslatableText(L"hbp.title");
	StartWindow() {
		Button* start = dynamic_cast<Button*>(widgets.emplace_back(std::move(Button(0, 0.1, 0.4, 0.08, Location::CENTER, LiteralText(L"\\#ff44ee66Exit")))).ptr());
		start->onTick = [start, this](int) { if (start->containsMouse()) game.getFloatWindow().push(L"\\#ff44ee66退出游戏"_renderable); };
		start->mouseUp = [](int) {
			game.setWindow(&ConfirmWindow::of(LiteralText(L"是否\\#ff44ee66退出游戏\\r？"))->requireCancel().requireConfirm([](Button& confirm) {
				confirm.mouseUp = [](int) { DestroyWindow(MainWindowHandle); };
				confirm.onTick = [&confirm](int) {
					if (confirm.containsMouse()) {
						game.getFloatWindow().push(L"\\#ff44ee66确定退出游戏"_renderable);
						confirm.backgroundColor.hover = confirm.animation.adaptsColor(0x99008800, 0x9900ff00);
					}
				};
			}));
		};
		Button* test = dynamic_cast<Button*>(widgets.emplace_back(std::move(Button(0, 0.2, 0.4, 0.08, Location::CENTER, LiteralText(L"\\#ff4488eeOptions")))).ptr());
		Button* exit = dynamic_cast<Button*>(widgets.emplace_back(std::move(Button(0, 0.3, 0.4, 0.08, Location::CENTER, LiteralText(L"\\#ffee0000Start")))).ptr());
		exit->onTick = [exit](int) { if (exit->containsMouse()) game.getFloatWindow().push(L"\\#ffee0000开始游戏"_renderable); };
	}

public:
	static StartWindow* create() noexcept { return allocatedFor(new StartWindow()); }
	void onClose() override { pop(); }
	void render() const noexcept override {
		fontManager.getDefault().drawCenter(title.getRenderableString(), 0, 0, renderer.getWidth(), renderer.getHeight());
		Window::render();
	}
};
