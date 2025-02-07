//
// Created by EmsiaetKadosh on 25-1-14.
//

#include "Game.h"



Game::Game() : floatWindow{ new FloatWindow() } { Logger.put(L"Game created"); }

Game::~Game() { delete floatWindow; }

inline Game game = Game();
