#include "GameObject.h"
#include "InputManager.h"


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

	if (rigidbody)
	{
		float speed = 3.0f;
		float move = speed * dt;
		InputManager* inputManager = InputManager::Instance();
		if ((int)inputManager->GetKeyState(VK_DOWN) > 0)
		{
			rigidbody->SetXVelocity(-1);
		}
		else if ((int)inputManager->GetKeyState(VK_UP) > 0)
		{
			rigidbody->SetXVelocity(1);
		}
		else
		{
			rigidbody->SetXVelocity(0);
		}
		if ((int)inputManager->GetKeyState(VK_LEFT) > 0)
		{
			rigidbody->SetZVelocity(1);
		}
		else if ((int)inputManager->GetKeyState(VK_RIGHT) > 0)
		{
			rigidbody->SetZVelocity(-1);
		}
		else
		{
			rigidbody->SetZVelocity(0);
		}
		rigidbody->Update(dt, position);

		SetRotation(rotation * XMMatrixRotationX(rigidbody->GetVelocity().z * 0.005f) * XMMatrixRotationZ(-rigidbody->GetVelocity().x * 0.005f));
	}
}