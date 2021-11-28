#pragma once
#include "MeshComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"
#include "RigidbodyComponent.h"
#include "MovementComponent.h"

enum class ObjectType
{
	NONE = 0,
	SPHERE = 1,
	CUBE = 2,
	SLOPE = 3,
	SLOPE90 = 4,
	SLOPE180 = 5,
	SLOPE270 = 6,
	GOAL = 7,
};

class GameObject
{
public:
	GameObject();
	GameObject(int);
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

	ObjectType GetObjectType();

	void SetPosition(VECTOR3);
	void SetRotation(const XMMATRIX&);

	VECTOR3 GetPosition();

	void Update(float dt);
private:

	ObjectType type = ObjectType::NONE;

	MeshComponent* meshComponent;
	CameraComponent* cameraComponent;
	LightComponent* lightComponent;
	RigidbodyComponent* rigidbody;
	MovementComponent* movementComponent;

	VECTOR3 position;
	XMMATRIX rotation;
};

