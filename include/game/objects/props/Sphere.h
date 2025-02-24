#pragma once

#include "core/math/Vector.h"
#include <cassert>

class Sphere {
public:
	explicit Sphere(float r = 1.0f) : center(0.0f), radius(r) {}

	Sphere(const Vector3 &pos, float r) : center(pos), radius(r) {}

	// Transformations to and from sphere coordinates
	[[nodiscard]] Matrix4 UnitToLocal() const {
		assert(radius > 0.0f);
		return Matrix4::Trans(center) * Matrix4::Scale(radius);
	}

	[[nodiscard]] Matrix4 LocalToUnit() const {
		assert(radius > 0.0f);
		return Matrix4::Scale(1.0f / radius) * Matrix4::Trans(-center);
	}

	Vector3 center;
	float radius;
};
