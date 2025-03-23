//
// Created by EmsiaetKadosh on 25-1-14.
//

#include "Game.h"

#include "entity\Entity.h"
#include "world\World.h"
#include "..\ui\xWindows.h"

void Game::initialize() {
	worldManager = allocatedFor(new WorldManager);
	entityManager = allocatedFor(new EntityManager);
	setWindow(StartWindow::create());
	windows.onResize();
}

Game::Game() : caption{ allocatedFor(new CaptionWindow()) }, floatWindow{ allocatedFor(new FloatWindow()) } {
	Logger.put(L"Game created");
	random.seed(timeGetTime());
}

Game::~Game() {
	setWindow(nullptr);
	delete deallocating(floatWindow);
	gc.pack();
	gc.collect();
	delete deallocating(caption);
	gc.pack();
	gc.collect();
	delete deallocating(worldManager);
	gc.pack();
	gc.collect();
	delete deallocating(entityManager);
	gc.pack();
	gc.collect();
}

void Game::render(const double tickDelta) const noexcept {
	if (renderer.checkResizing()) return;
	renderer.gameStartRender();
	if (worldManager->current) worldManager->current->render(tickDelta);
	caption->render(tickDelta);
	hud.render(tickDelta);
	windows.render(tickDelta);
	floatWindow->render(tickDelta);
	renderer.gameEndRender();
	gc.pack();
}

void Game::tick() noexcept {
	++currentTick;
	if (worldManager->current) worldManager->current->tick();
	floatWindow->clear();
	floatWindow->tick();
	caption->tick();
	hud.tick();
	windows.tick();
	floatWindow->update();
	tasks.runAll();
	gc.collect();
}

inline Game game = Game();
