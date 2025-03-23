//
// Created by EmsiaetKadosh on 25-3-21.
//

#pragma once

#include "..\..\utils\math.h"
#include "..\..\render\Renderer.h"
#include "..\world\Location.h"
#include "Damage.h"

class Entity;
class [[carlbeks::predecl, carlbeks::defineat("World.h")]] World;
class EntityManager;

using EntityID = QWORD;

interface IDamageable {
protected:
	double maxHealth = 0;
	double health = 0;

public:
	virtual ~IDamageable() = default;
	virtual void Damage(Damage&) = 0;
	virtual void onDeath() = 0;
};

interface IArtificialIntelligent {
protected:
	virtual ~IArtificialIntelligent() = default;
	virtual void aiProcess() {}
};

class Entity : public IRenderable, public ITickable {
	friend class World;
	friend class EntityManager;
	EntityID idEntity = 0;
	World* world = nullptr;

protected:
	Location location;
	Vector2D velocity;
	double maxSpeed = 1.0;

	Entity(const Vector2D& location) : location(location) {}
	~Entity() override = default;

public:
	virtual void onRemove() = 0;
	virtual void onEnterWorld(World* world, WorldTransportReason reason) {}
	virtual void onExitWorld(World* world, WorldTransportReason reason) {}

	void setVelocity(const Vector2D& velocity) noexcept { this->velocity = velocity; }
	[[nodiscard]] const Location& getLocation() const noexcept { return this->location; }
	[[nodiscard]] const Location& getLocation(const double tickDelta) const noexcept { return Location(this->location.getPosition() + this->velocity * tickDelta, this->location.getWorld()); }
	[[nodiscard]] Vector2D getVelocity() const noexcept { return this->velocity; }
	[[nodiscard]] double getMaxSpeed() const noexcept { return this->maxSpeed; }
};

class Enemy : public Entity, public IDamageable, public IArtificialIntelligent {
protected:
	Enemy(const Vector2D& location) : Entity(location) {}
};

class EntityManager {
	friend class Game;
	EntityID nextID = 0;
	Map<EntityID, Entity*> entities;
	EntityManager() = default;
	~EntityManager() { for (auto& [id, entity] : entities) { entity->onRemove(); } }

public:
	int addEntity(Entity* entity) {
		if (!entity) Failed();
		if (entity->idEntity) Failed();
		entity->idEntity = ++nextID;
		entities.emplace(entity->idEntity, entity);
		Success();
	}

	int removeEntity(Entity* entity) {
		if (!entity) Failed();
		if (entity->idEntity) Failed();
		if (entity->getLocation().getWorld()) Failed(); // 确保必须已经从其他世界移除
		entities.erase(entity->idEntity);
		entity->onRemove();
		Success();
	}
};
