//
// Created by EmsiaetKadosh on 25-1-18.
//

#pragma once

#include "Window.h"

class StartWindow final : public Window {
	TranslatableText title = TranslatableText(L"hbp.title");

	StartWindow() {
		Button* start = dynamic_cast<Button*>(widgets.emplace_back(std::move(Button(0, 0.1, 0.4, 0.08, Location::CENTER, LiteralText(L"\\f\4\\#ff44ee66Exit")))).ptr());
		start->onTick = [](const Widget& self, MouseButtonCode) { if (self.containsMouse()) game.getFloatWindow().push(L"\\#ff44ee66退出游戏"_renderable); };
		start->mouseClick = [](Widget&, MouseButtonCode) {
			game.setWindow(&ConfirmWindow::of(LiteralText(L"是否\\#ff44ee66退出游戏\\r？"))->requireCancel().requireConfirm([](Button& confirm) {
				confirm.mouseClick = [](Widget&, MouseButtonCode) { DestroyWindow(MainWindowHandle); };
				confirm.onTick = [](Widget& self, MouseButtonCode) {
					if (self.containsMouse()) {
						game.getFloatWindow().push(L"\\#ff44ee66确定退出游戏"_renderable);
						self.backgroundColor.hover = static_cast<Button&>(self).animation.adaptsColor(0x99008800, 0x9900ff00);
					}
				};
			}));
		};
		Button* optn = dynamic_cast<Button*>(widgets.emplace_back(std::move(Button(0, 0.2, 0.4, 0.08, Location::CENTER, LiteralText(L"\\f\4\\#ff4488eeOptions")))).ptr());
		optn->onTick = [](const Widget& self, MouseButtonCode) { if (self.containsMouse()) game.getFloatWindow().push(L"\\#ff4488ee设置"_renderable); };
		Button* exit = dynamic_cast<Button*>(widgets.emplace_back(std::move(Button(0, 0.3, 0.4, 0.08, Location::CENTER, LiteralText(L"\\f\4\\#ffee0000Start")))).ptr());
		exit->onTick = [](const Widget& self, MouseButtonCode) { if (self.containsMouse()) game.getFloatWindow().push(L"\\#ffee0000开始游戏"_renderable); };
		exit->mouseClick = [](Widget&, MouseButtonCode) {
			game.setWindow(&ConfirmWindow::of(LiteralText(L"是否\\#ffee0000退出游戏\\r？"))->requireCancel().requireConfirm([](Button& confirm) {
				confirm.mouseClick = [](Widget&, MouseButtonCode) { DestroyWindow(MainWindowHandle); };
				confirm.onTick = [](Widget& self, MouseButtonCode) {
					if (self.containsMouse()) {
						game.getFloatWindow().push(L"\\#ff44ee66确定退出游戏"_renderable);
						self.backgroundColor.hover = static_cast<Button&>(self).animation.adaptsColor(0x99008800, 0x9900ff00);
					}
				};
			}));
		};
	}

public:
	static StartWindow* create() noexcept { return allocatedFor(new StartWindow()); }
	void onClose() override { pop(); }

	void render() const noexcept override {
		fontManager.getDefault().drawCenter(title.getRenderableString(), 0, 0, renderer.getWidth(), renderer.getHeight());
		Window::render();
	}
};
