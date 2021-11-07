#pragma once
#include "MeshComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"
#include "RigidbodyComponent.h"

class GameObject
{
public:
	GameObject();
	~GameObject() = default;

	void AddMeshComponent(HWND, ID3D11Device*);
	void AddCameraComponent();
	void AddLightComponent(XMVECTOR);
	void AddRigidbody(float);

	MeshComponent* GetMeshComponent();
	CameraComponent* GetCameraComponent();
	LightComponent* GetLightComponent();
	RigidbodyComponent* GetRigidbody();

	void SetPosition(VECTOR3);
	void SetRotation(const XMMATRIX&);

	VECTOR3 GetPosition();

	void Update(float dt);
private:
	MeshComponent* meshComponent;
	CameraComponent* cameraComponent;
	LightComponent* lightComponent;
	RigidbodyComponent* rigidbody;

	VECTOR3 position;
	XMMATRIX rotation;
};

