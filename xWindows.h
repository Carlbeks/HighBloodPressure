//
// Created by EmsiaetKadosh on 25-1-18.
//

#pragma once

#include "Window.h"

class StartWindow : public Window {
public:
	StartWindow() {
		Button* button = new Button(0, 0, 0.4, 0.08, Location::CENTER, TranslatableText(L"123"));
		widgets.push_back(button);
	}
};
