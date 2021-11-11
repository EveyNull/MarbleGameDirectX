#include "RigidbodyComponent.h"

RigidbodyComponent::RigidbodyComponent(float newMass)
{
	mass = newMass;
	velocity = { 0, 0, 0 };
	angularVelocity = { 0, 0, 0 };
	force = { 0, 0, 0 };
	torque = { 0, 0, 0 };
}

void RigidbodyComponent::Update(float dt, VECTOR3& position)
{
	if (gravity)
	{
		float gravityForce = -9.81 * mass * dt;
		VECTOR3 gravity = { 0, gravityForce, 0 };
		velocity += gravity;
	}
}

void RigidbodyComponent::SetVelocity(const VECTOR3& newVelocity)
{
	force = { 0, 0, 0 };
	velocity = newVelocity;
}

void RigidbodyComponent::SetXVelocity(const float& newX)
{
	force = { 0, 0, 0 };
	velocity.x = newX;
}

void RigidbodyComponent::SetYVelocity(const float& newY)
{
	force = { 0, 0, 0 };
	velocity.y = newY;
}

void RigidbodyComponent::SetZVelocity(const float& newZ)
{
	force = { 0, 0, 0 };
	velocity.z = newZ;
}

void RigidbodyComponent::AddVelocity(const VECTOR3& newForce)
{
	velocity += newForce;
}

VECTOR3 RigidbodyComponent::GetForce()
{
	return force;
}

VECTOR3 RigidbodyComponent::GetVelocity()
{
	return velocity;
}

void RigidbodyComponent::EnableGravity(bool enable)
{
	gravity = enable;
}