//
// Created by EmsiaetKadosh on 25-3-21.
//

#pragma once

#include "..\..\utils\math.h"
#include "..\world\Location.h"
#include "Damage.h"

class Entity;
class World;

interface IDamageable {
protected:
	double maxHealth = 0;
	double health = 0;

public:
	virtual ~IDamageable() = default;
	virtual void Damage(Damage&) = 0;
	virtual void onDeath() = 0;
};

interface IMoveable {
protected:
	double maxSpeed = 1.0;
	Vector2D velocity;

	virtual ~IMoveable() = default;
public:
	void setVelocity(const Vector2D& velocity) { this->velocity = velocity; }
	[[nodiscard]] Vector2D getVelocity() const { return this->velocity; }
};

interface IArtificialIntelligent {
protected:
	virtual ~IArtificialIntelligent() = default;
	virtual void aiProcess() {}
};

class Entity {
	friend class World;
	Location location;
	QWORD id = 0;

protected:
	Entity(const Location& location) : location(location) {}
	Entity(Location&& location) : location(std::move(location)) {}
	virtual ~Entity() = default;

public:
	virtual void tick() {}
	virtual void onRemove() {}
};

class Enemy : public Entity, public IDamageable, public IMoveable, public IArtificialIntelligent {
protected:
	Enemy(const Location& location) : Entity(location) {}
	Enemy(Location&& location) : Entity(std::move(location)) {}
};
