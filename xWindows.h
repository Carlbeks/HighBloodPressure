//
// Created by EmsiaetKadosh on 25-1-18.
//

#pragma once

#include "Window.h"

class StartWindow final : public Window {
	StartWindow() {
		Button* start = dynamic_cast<Button*>(widgets.emplace_back(std::move(Button(0, 0, 0.4, 0.08, Location::CENTER, LiteralText(L"\\#ff44ee66Exit")))).ptr());
		start->onTick = [start](int) { if (start->containsMouse()) game.getFloatWindow().push(L"\\#ff44ee66退出游戏"_renderable); };
		start->mouseUp = [](int) { game.setWindow(&ConfirmWindow::create(LiteralText(L"是否\\#ff44ee66退出游戏\\#ff44ee66？"))->requireCancel().requireConfirm()); };
		Button* test = dynamic_cast<Button*>(widgets.emplace_back(std::move(Button(0, 0.1, 0.4, 0.08, Location::CENTER, LiteralText(L"\\#ff4488ee\\f\1Font1\\f\2Font2\\f\1Font1\\f\3Font3\\f\1Font1\\f\4Font4\\f\1Font1\\f\5Font5\\f\1Font1\\f\6Font6\\f\1Font1\\f\7Font7\\f\1Font1\\f\10Font8\\f\1Font1\\f\11Font9")))).ptr());
		Button* exit = dynamic_cast<Button*>(widgets.emplace_back(std::move(Button(0, 0.2, 0.4, 0.08, Location::CENTER, LiteralText(L"\\#ffee0000Start")))).ptr());
		exit->onTick = [exit](int) { if (exit->containsMouse()) game.getFloatWindow().push(L"\\#ffee0000开始游戏"_renderable); };
	}

public:
	static StartWindow* create() noexcept { return new StartWindow(); }
	void onClose() override { pop(); }
};
