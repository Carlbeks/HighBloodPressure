//
// Created by EmsiaetKadosh on 25-1-14.
//

#include "Game.h"


Game::Game() : caption{ allocatedFor(new CaptionWindow()) }, floatWindow{ allocatedFor(new FloatWindow()) } {
	Logger.put(L"Game created");
	random.seed(timeGetTime());
}

Game::~Game() {
	setWindow(nullptr);
	delete deallocating(floatWindow);
	delete deallocating(caption);
}

inline Game game = Game();
