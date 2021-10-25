#pragma once
#include "MeshComponent.h"
#include "CameraComponent.h"

class GameObject
{
public:
	GameObject() = default;
	~GameObject() = default;

	void AddMeshComponent(HWND hWnd, ID3D11Device* device);
	void AddCameraComponent();

	MeshComponent* GetMeshComponent();
	CameraComponent* GetCameraComponent();

	void SetPosition(VECTOR3);
	void SetRotation(VECTOR3);

	void Update(float dt);
private:
	MeshComponent* meshComponent;
	CameraComponent* cameraComponent;

	VECTOR3 position;
	VECTOR3 rotation;
};

