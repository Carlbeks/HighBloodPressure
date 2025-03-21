//
// Created by EmsiaetKadosh on 25-3-21.
//

#pragma once

#include "..\..\def.h"
#include "..\entity\Entity.h"

class World {
	QWORD idEntity = 0;
	Map<QWORD, Entity*> entities;
	using IterEntity = Map<QWORD, Entity*>::const_iterator;

public:
	World() = default;

	int addEntity(Entity* entity) {
		if (!entity) Failed();
		if (entity->id) Failed();
		entities.emplace(++idEntity, entity);
		Success();
	}

	int removeEntity(Entity* entity) {
		if (!entity) Failed();
		if (!entity->id) Failed();
		const IterEntity iter = entities.find(entity->id);
		if (iter != entities.end()) Failed();
		entities.erase(iter);
		entity->onRemove();
		entity->id = 0;
		Success();
	}
};

class WorldManager {
public:
};
