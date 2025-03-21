//
// Created by EmsiaetKadosh on 25-3-21.
//

#pragma once

#include "..\warnings.h"

class Vector3D;
class Vector2D;

class Vector3D {
	double x, y, z;

public:
	Vector3D(const double x, const double y, const double z) noexcept : x(x), y(y), z(z) {}
	Vector3D() noexcept : x(0), y(0), z(0) {}
	Vector3D(const Vector3D& other) noexcept = default;
	Vector3D(Vector3D&& other) noexcept = default;
	Vector3D& operator=(const Vector3D& other) noexcept = default;
	Vector3D& operator=(Vector3D&& other) noexcept = default;
	~Vector3D() noexcept = default;

	[[nodiscard]] double getX() const noexcept { return x; }
	[[nodiscard]] double getY() const noexcept { return y; }
	[[nodiscard]] double getZ() const noexcept { return z; }

	[[nodiscard]] Vector3D operator+(const Vector3D& other) const noexcept { return Vector3D(x + other.x, y + other.y, z + other.z); }
	[[nodiscard]] Vector3D operator-(const Vector3D& other) const noexcept { return Vector3D(x - other.x, y - other.y, z - other.z); }
	[[nodiscard]] Vector3D operator*(const double scalar) const noexcept { return Vector3D(x * scalar, y * scalar, z * scalar); }
	[[nodiscard]] Vector3D operator/(const double scalar) const noexcept { return Vector3D(x / scalar, y / scalar, z / scalar); }
	[[nodiscard]] double operator*(const Vector3D& other) const noexcept { return x * other.x + y * other.y + z * other.z; }

	Vector3D& operator+=(const Vector3D& other) noexcept {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3D& operator-=(const Vector3D& other) noexcept {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vector3D& operator*=(const double scalar) noexcept {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	Vector3D& operator/=(const double scalar) noexcept {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	[[nodiscard]] Vector3D operator-() const noexcept { return Vector3D(-x, -y, -z); }
	bool operator==(const Vector3D& other) const noexcept { return x == other.x && y == other.y && z == other.z; }
	bool operator!=(const Vector3D& other) const noexcept { return x != other.x || y != other.y || z != other.z; }

	[[nodiscard]] double length() const noexcept { return std::sqrt(x * x + y * y + z * z); }

	[[nodiscard]] Vector3D getNormalized() const noexcept {
		if (x == 0.0 && y == 0.0 && z == 0) return Vector3D(0.0, 0.0, 0.0);
		const double length = this->length();
		return Vector3D(x / length, y / length, z / length);
	}

	Vector3D& normalize() noexcept {
		if (x == 0 && y == 0 && z == 0) return *this;
		const double length = this->length();
		x /= length;
		y /= length;
		z /= length;
		return *this;
	}

	[[nodiscard]] double dot(const Vector3D& other) const noexcept { return x * other.x + y * other.y + z * other.z; }
	[[nodiscard]] Vector3D cross(const Vector3D& other) const noexcept { return Vector3D(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x); }
};

class Vector2D {
	double x, y;

public:
	Vector2D(const double x, const double y) noexcept : x(x), y(y) {}
	Vector2D() noexcept : x(0), y(0) {}
	Vector2D(const Vector2D& other) noexcept = default;
	Vector2D(Vector2D&& other) noexcept = default;
	Vector2D& operator=(const Vector2D& other) noexcept = default;
	Vector2D& operator=(Vector2D&& other) noexcept = default;
	~Vector2D() noexcept = default;

	[[nodiscard]] double getX() const noexcept { return x; }
	[[nodiscard]] double getY() const noexcept { return y; }

	[[nodiscard]] Vector2D operator+(const Vector2D& other) const noexcept { return Vector2D(x + other.x, y + other.y); }
	[[nodiscard]] Vector2D operator-(const Vector2D& other) const noexcept { return Vector2D(x - other.x, y - other.y); }
	[[nodiscard]] Vector2D operator*(const double scalar) const noexcept { return Vector2D(x * scalar, y * scalar); }
	[[nodiscard]] Vector2D operator/(const double scalar) const noexcept { return Vector2D(x / scalar, y / scalar); }
	[[nodiscard]] double operator*(const Vector2D& other) const noexcept { return x * other.x + y * other.y; }

	Vector2D& operator+=(const Vector2D& other) noexcept {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2D& operator-=(const Vector2D& other) noexcept {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2D& operator*=(const double scalar) noexcept {
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2D& operator/=(const double scalar) noexcept {
		x /= scalar;
		y /= scalar;
		return *this;
	}

	[[nodiscard]] Vector2D operator-() const noexcept { return Vector2D(-x, -y); }
	bool operator==(const Vector2D& other) const noexcept { return x == other.x && y == other.y; }
	bool operator!=(const Vector2D& other) const noexcept { return x != other.x || y != other.y; }

	[[nodiscard]] double length() const noexcept { return std::sqrt(x * x + y * y); }

	[[nodiscard]] Vector2D getNormalized() const noexcept {
		if (x == 0.0 && y == 0.0) return Vector2D(0.0, 0.0);
		const double length = this->length();
		return Vector2D(x / length, y / length);
	}

	Vector2D& normalize() noexcept {
		if (x == 0 && y == 0) return *this;
		const double length = this->length();
		x /= length;
		y /= length;
		return *this;
	}

	[[nodiscard]] double dot(const Vector2D& other) const noexcept { return x * other.x + y * other.y; }
	[[nodiscard]] Vector3D cross(const Vector2D& other) const noexcept { return Vector3D(0, 0, x * other.y - y * other.x); }
};
