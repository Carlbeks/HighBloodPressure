//
// Created by EmsiaetKadosh on 25-3-21.
//

#pragma once

#include "Entity.h"

class Player : public Entity, public IDamageable {
public:
	Player(const Vector2D& location) : Entity(location) {}
};
