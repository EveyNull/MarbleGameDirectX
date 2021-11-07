#pragma once

#include "Vector.h"

class RigidbodyComponent
{
public:
	RigidbodyComponent(float);
	~RigidbodyComponent() = default;

	void Update(float dt, VECTOR3& position);

	void SetVelocity(const VECTOR3& newVelocity);

	void SetXVelocity(const float& newX);
	void SetZVelocity(const float& newY);
	void SetYVelocity(const float& newZ);

	void AddVelocity(const VECTOR3& force);

	VECTOR3 GetVelocity();
	VECTOR3 GetForce();

	void EnableGravity(bool);
	

private:
	VECTOR3 velocity;
	VECTOR3 angularVelocity;
	VECTOR3 force;
	VECTOR3 torque;
	float mass;
	bool gravity = true;
};

