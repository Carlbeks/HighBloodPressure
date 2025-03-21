//
// Created by EmsiaetKadosh on 25-3-21.
//

#pragma once

#include "..\..\utils\math.h"

using WorldID = QWORD;
class Location;
class BlockLocation;

class Location {
	Vector2D position;
	WorldID idWorld;

public:
	Location(const Vector2D& position, const WorldID idWorld) : position(position), idWorld(idWorld) {}
	Location(Vector2D&& position, const WorldID idWorld) : position(position), idWorld(idWorld) {}
	[[nodiscard]] Vector2D getPosition() const { return position; }
	[[nodiscard]] WorldID getWorld() const { return idWorld; }
	[[nodiscard]] double getX() const { return position.getX(); }
	[[nodiscard]] double getY() const { return position.getY(); }
	[[nodiscard]] BlockLocation getBlockLocation() const;
};

class BlockLocation {
	long long x, y;
	WorldID idWorld;

public:
	BlockLocation(const long long x, const long long y, const WorldID idWorld) : x(x), y(y), idWorld(idWorld) {}
	BlockLocation(const Vector2D& position, const WorldID idWorld) : x(static_cast<long long>(std::floor(position.getX()))), y(static_cast<long long>(std::floor(position.getY()))), idWorld(idWorld) {}
	[[nodiscard]] Vector2D getPosition() const { return Vector2D(static_cast<double>(x), static_cast<double>(y)); }
	[[nodiscard]] WorldID getWorld() const { return idWorld; }
	[[nodiscard]] long long getX() const { return x; }
	[[nodiscard]] long long getY() const { return y; }
	[[nodiscard]] Location toLocation() const { return Location({ static_cast<double>(x), static_cast<double>(y) }, idWorld); }
};

inline BlockLocation Location::getBlockLocation() const { return BlockLocation(position, idWorld); }

