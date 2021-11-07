#pragma once
#include "GameObject.h"
#include "Vector.h"

class CollisionManager
{
public:
	CollisionManager(GameObject** geometry, int);
	~CollisionManager() = default;
	
	bool CheckSphereOnMeshes(GameObject* sphere, VECTOR3& outNormal, VECTOR3& collisionPoint);
private:
	GameObject** staticGeometry;
	int geometryNumber;

	bool SphereOnMeshCollision(MeshComponent* sphereMesh, MeshComponent* meshOther, VECTOR3& outNormal);
	bool SphereOnTriangleCollision(MeshComponent* sphereMesh, VECTOR3* triangle, VECTOR3& outNormal);
};

