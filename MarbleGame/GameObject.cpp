#include "GameObject.h"
#include "InputManager.h"
#include <algorithm>


GameObject::GameObject()
{
	position = { 0.0f, 0.0f, 0.0f };
	rotation = XMMatrixIdentity();
}

void GameObject::AddMeshComponent(HWND hWnd, ID3D11Device* device)
{
	meshComponent = new MeshComponent(hWnd);
	meshComponent->TranslateMesh(position);
}

void GameObject::AddCameraComponent()
{
	cameraComponent = new CameraComponent();
}

void GameObject::AddLightComponent(XMVECTOR direction)
{
	lightComponent = new LightComponent(direction, { 1.0f, 1.0f, 1.0f, 1.0f });
}

void GameObject::AddRigidbody(float mass)
{
	rigidbody = new RigidbodyComponent(mass);
}

void GameObject::AddMovementComponent(float* ptr)
{
	movementComponent = new MovementComponent(ptr);
}

MeshComponent* GameObject::GetMeshComponent()
{
	return meshComponent;
}

CameraComponent* GameObject::GetCameraComponent()
{
	return cameraComponent;
}

LightComponent* GameObject::GetLightComponent()
{
	return lightComponent;
}

MovementComponent* GameObject::GetMoveComponent()
{
	return movementComponent;
}

RigidbodyComponent* GameObject::GetRigidbody()
{
	return rigidbody;
}

VECTOR3 GameObject::GetPosition()
{
	return position;
}

void GameObject::SetPosition(VECTOR3 newPos)
{
	position = newPos;
	if (meshComponent)
	{
		meshComponent->TranslateMesh(position);
	}
	if (cameraComponent)
	{
		XMVECTOR newCameraPos =
		{
			newPos.x,
			newPos.y,
			newPos.z
		};
		cameraComponent->SetPosition(newCameraPos);
	}
}

void GameObject::SetRotation(const XMMATRIX& newRot)
{
	rotation = newRot;
	if (meshComponent)
	{
		meshComponent->RotateMesh(rotation);
	}
}

void GameObject::Update(float dt)
{
	if (cameraComponent)
	{
		VECTOR2 mouseMovement = InputManager::Instance()->GetMouseMovement();
		if (mouseMovement.x != 0 && mouseMovement.y != 0)
		{
			VECTOR3 currentRot = cameraComponent->GetRotation();
			VECTOR3 newRot =
				{
					currentRot.x ,
					currentRot.y + mouseMovement.x * dt * 2,
					currentRot.z
				};
			cameraComponent->SetRotation(newRot);
		}

	}
	if (movementComponent)
	{
		movementComponent->Update(dt, rigidbody);
	}
	if (rigidbody)
	{
		rigidbody->Update(dt, position);

		SetRotation(rotation * XMMatrixRotationX(rigidbody->GetVelocity().z * 0.005f) * XMMatrixRotationZ(-rigidbody->GetVelocity().x * 0.005f));
	}
}