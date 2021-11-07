#pragma once
#include "MeshComponent.h"
class CollisionComponent
{
public:
	CollisionComponent(MeshComponent*);
	~CollisionComponent() = default;
private:
	MeshComponent* mesh;

	VECTOR3 boundingBox[8];
};

