//
// Created by EmsiaetKadosh on 25-3-21.
//

#pragma once

#include "Entity.h"

class Player : public Entity, public IMoveable, public IDamageable {
public:
	Player(const Location& location) : Entity(location) {}
	Player(Location&& location) : Entity(std::move(location)) {}
};
