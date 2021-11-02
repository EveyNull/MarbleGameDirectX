#include "GameObject.h"
#include "InputManager.h"


void GameObject::AddMeshComponent(HWND hWnd, ID3D11Device* device)
{
	meshComponent = new MeshComponent(hWnd);
}

void GameObject::AddCameraComponent()
{
	cameraComponent = new CameraComponent();
	position = { 0.0f, 0.0f, -10.0f };
	rotation = { 0.0f, 0.0f, 0.0f };
}

void GameObject::AddLightComponent(XMVECTOR direction)
{
	lightComponent = new LightComponent(direction, { 1.0f, 1.0f, 1.0f, 1.0f });
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

void GameObject::SetPosition(VECTOR3 newPos)
{
	position = newPos;
}

void GameObject::Update(float dt)
{

	if (meshComponent)
	{
		/*
		rotation.y += 0.5f * dt;
		if (rotation.y > 360.0f) rotation.y -= 360.0f;
		rotation.x += 0.5f * dt;
		if (rotation.x > 360.0f) rotation.x -= 360.0f;
		meshComponent->RotateMesh({ rotation.x, rotation.y, rotation.z });
		*/
		meshComponent->TranslateMesh({ position.x, position.y, position.z });
	}

	if (cameraComponent)
	{
		float speed = 1.0f;
		float move = speed * dt;
		float rot = speed * dt * 5.0f;
		InputManager* inputManager = InputManager::Instance();
		if ((int)inputManager->GetKeyState(VK_LEFT) > 0)
		{
			position.x -= move;
		}
		if ((int)inputManager->GetKeyState(VK_RIGHT) > 0)
		{
			position.x += move;
		}
		if ((int)inputManager->GetKeyState(VK_UP) > 0)
		{
			position.y += move;
		}
		if ((int)inputManager->GetKeyState(VK_DOWN) > 0)
		{
			position.y -= move;
		}

		if ((int)inputManager->GetKeyState(87) > 0)
		{
			rotation.x += rot;
		}

		if ((int)inputManager->GetKeyState(83) > 0)
		{
			rotation.x -= rot;
		}

		if ((int)inputManager->GetKeyState(65) > 0)
		{
			rotation.y -= rot;
		}

		if ((int)inputManager->GetKeyState(68) > 0)
		{
			rotation.y += rot;
		}
		cameraComponent->SetPosition({ position.x, position.y, position.z });
		cameraComponent->SetRotation(rotation);
	}
}