//
// Created by EmsiaetKadosh on 25-3-21.
//

#pragma once

#include "..\..\def.h"
#include "..\..\utils\gc.h"
#include "Location.h"

class [[carlbeks::predecl, carlbeks::defineat("World.h")]] World;

class Block : public IRenderable, public ITickable {
	friend class Garbage<Block>;
	friend class World;
	World* world = nullptr;
	BlockLocation location;

public:
	Block(const BlockLocation& location) : location(location) { this->location.setWorld(0); }
	[[nodiscard]] const BlockLocation& getLocation() const { return location; }
	virtual void onEnterWorld(World* world, WorldTransportReason reason) {}
	virtual void onExitWorld(World* world, WorldTransportReason reason) {}
	virtual void onRemove() { gc.submit<Block>(this); }
	/**
	 * 渲染方块影。方块影会覆盖所有的方块。
	 */
	virtual void renderShadow() const noexcept {}
	/**
	 * 渲染方块本体。渲染范围不应当超过方块占据的范围。
	 */
	void render(double tickDelta) const noexcept override = 0;
};

class PureBarrierBlock final : public Block {
	PureBarrierBlock(const BlockLocation& location) : Block(location) {}
	~PureBarrierBlock() override {}

public:
	void render(double tickDelta) const noexcept override { renderer.fillWorldBlock(getLocation(), 0xffeeeeee); }
	void tick() noexcept override {}
	static PureBarrierBlock* create(const BlockLocation& location) { return allocatedFor(new PureBarrierBlock(location)); }
};
