//
// Created by EmsiaetKadosh on 25-1-14.
//

#include "Game.h"

#include "world\World.h"
#include "..\ui\xWindows.h"

void Game::initialize() {
	worldManager = allocatedFor(new WorldManager);
	game.setWindow(StartWindow::create());
}

Game::Game() : caption{ allocatedFor(new CaptionWindow()) }, floatWindow{ allocatedFor(new FloatWindow()) } {
	Logger.put(L"Game created");
	random.seed(timeGetTime());
}

Game::~Game() {
	setWindow(nullptr);
	delete deallocating(floatWindow);
	delete deallocating(caption);
	delete deallocating(worldManager);
}

inline Game game = Game();
