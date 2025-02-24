#pragma once

#include "game/objects/base/Object.h"
#include "game/objects/interactive/Portal.h"
#include "game/objects/props/Sphere.h"

class Physical : public Object {
public:
	Physical();

	~Physical() override = default;

	void Reset() override;

	void Update() override;

	virtual void OnCollide(Object &other, const Vector3 &push);

	void SetPosition(const Vector3 &_pos) {
		pos = _pos;
		prev_pos = _pos;
	}

	bool TryPortal(const Portal &portal);

	Physical *AsPhysical() override { return this; }

	Vector3 gravity{};
	Vector3 velocity{};
	float bounce{};
	float friction{};
	float high_friction{};
	float drag{};

	Vector3 prev_pos{};

	std::vector<Sphere> hitSpheres;
};
