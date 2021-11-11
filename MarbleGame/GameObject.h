#pragma once
#include "MeshComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"
#include "RigidbodyComponent.h"
#include "MovementComponent.h"

class GameObject
{
public:
	GameObject();
	~GameObject() = default;

	void AddMeshComponent(HWND, ID3D11Device*);
	void AddCameraComponent();
	void AddLightComponent(XMVECTOR);
	void AddRigidbody(float);
	void AddMovementComponent(float*);

	MeshComponent* GetMeshComponent();
	CameraComponent* GetCameraComponent();
	LightComponent* GetLightComponent();
	MovementComponent* GetMoveComponent();
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
	MovementComponent* movementComponent;

	VECTOR3 position;
	XMMATRIX rotation;
};

