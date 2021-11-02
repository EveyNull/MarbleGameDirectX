#pragma once
#include "MeshComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"

class GameObject
{
public:
	GameObject() = default;
	~GameObject() = default;

	void AddMeshComponent(HWND, ID3D11Device*);
	void AddCameraComponent();
	void AddLightComponent(XMVECTOR);

	MeshComponent* GetMeshComponent();
	CameraComponent* GetCameraComponent();
	LightComponent* GetLightComponent();

	void SetPosition(VECTOR3);
	void SetRotation(VECTOR3);

	void Update(float dt);
private:
	MeshComponent* meshComponent;
	CameraComponent* cameraComponent;
	LightComponent* lightComponent;

	VECTOR3 position;
	VECTOR3 rotation;
};

