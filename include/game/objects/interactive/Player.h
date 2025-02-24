#pragma once

#include "core/math/Vector.h"
#include "game/objects/base/Physical.h"

class Player : public Physical {
public:
	Player();

	~Player() override = default;

	void Reset() override;

	void Update() override;

	void OnCollide(Object &other, const Vector3 &push) override;

	void Look(float mouseDx, float mouseDy);

	void Move(float moveF, float moveL);

	[[nodiscard]] Matrix4 WorldToCam() const;

	[[nodiscard]] Matrix4 CamToWorld() const;

	[[nodiscard]] Vector3 CamOffset() const;

private:
	float cam_rx{};
	float cam_ry{};

	float bob_mag{};
	float bob_phi{};

	bool onGround{};
};
