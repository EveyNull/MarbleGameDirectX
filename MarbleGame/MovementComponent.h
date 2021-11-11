#pragma once

#include "RigidbodyComponent.h"
class MovementComponent
{
public:
	MovementComponent(float* camYawPtr);
	~MovementComponent() = default;

	void SetCanJump(bool);

	void Update(float dt, RigidbodyComponent* rigidbody);
private:
	float xVelocityMulti = 0.0f;
	float zVelocityMulti = 0.0f;

	float* camYawPtr;
	bool canJump = false;
};

