#include "MovementComponent.h"
#include "InputManager.h"
#include "MathHelper.h"
#include "RigidbodyComponent.h"
#include <Windows.h>
#include <DirectXMath.h>

float speedMinimum = 10.0f;
float speedMultiCap = 50.0f;
float velocityHardCap = 20.0f;

MovementComponent::MovementComponent(float* ptr)
{
	camYawPtr = ptr;
}

void MovementComponent::SetCanJump(bool jump)
{
	canJump = jump;
}

void MovementComponent::Update(float dt, RigidbodyComponent* rigidbody)
{
	float speed = 10.0f;
	float move = speed * dt;

	float camYaw = (*camYawPtr + 90) * (DirectX::XM_PI/180);
	float camYaw90 = (*camYawPtr) * (DirectX::XM_PI / 180);
	
	InputManager* inputManager = InputManager::Instance();
	if ((int)inputManager->GetControlState(InputAxis::VERTICAL) < 0)
	{
		MathHelper::MaxValue(xVelocityMulti, -speedMinimum);
		xVelocityMulti -= speed * dt;
	}
	else if ((int)inputManager->GetControlState(InputAxis::VERTICAL) > 0)
	{
		MathHelper::MinValue(xVelocityMulti, speedMinimum);
		xVelocityMulti += speed * dt;
	}
	else
	{
		xVelocityMulti = 0.0f;
	}

	if ((int)inputManager->GetControlState(InputAxis::HORIZONTAL) < 0)
	{
		MathHelper::MinValue(zVelocityMulti, speedMinimum);
		zVelocityMulti += speed * dt;
	}
	else if ((int)inputManager->GetControlState(InputAxis::HORIZONTAL) > 0)
	{
		MathHelper::MaxValue(zVelocityMulti, -speedMinimum);
		zVelocityMulti -= speed * dt;
	}
	else
	{
		zVelocityMulti = 0.0f;
	}

	MathHelper::ClampValue(xVelocityMulti, -speedMultiCap, speedMultiCap);
	MathHelper::ClampValue(zVelocityMulti, -speedMultiCap, speedMultiCap);



	float newVelX = rigidbody->GetVelocity().x + (-xVelocityMulti * std::cos(camYaw) * dt) + (zVelocityMulti * std::cos(camYaw90) * dt);
	float newVelZ = rigidbody->GetVelocity().z + (-xVelocityMulti * std::sin(camYaw) * dt) + (zVelocityMulti * std::sin(camYaw90) * dt);

	MathHelper::ClampValue(newVelX, -velocityHardCap, velocityHardCap);
	MathHelper::ClampValue(newVelZ, -velocityHardCap, velocityHardCap);

	rigidbody->SetXVelocity(newVelX * (1 - dt));
	rigidbody->SetZVelocity(newVelZ * (1 - dt));



	if ((int)inputManager->GetControlState(InputAxis::JUMP) > 0 && rigidbody->GetVelocity().y < 2.0f && canJump)
	{
		canJump = false;
		rigidbody->SetYVelocity(2400 * dt);
	}
}