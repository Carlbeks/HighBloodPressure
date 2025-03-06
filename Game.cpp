//
// Created by EmsiaetKadosh on 25-1-14.
//

#include "Game.h"


Game::Game() : caption{ new CaptionWindow() }, floatWindow{ new FloatWindow() } {
	Logger.put(L"Game created");
	random.seed(timeGetTime());
}

Game::~Game() {
	setWindow(nullptr);
	delete floatWindow;
	delete caption;
}

inline Game game = Game();
