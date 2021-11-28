#pragma once
#include "GameObject.h"
#include "Vector.h"

class CollisionManager
{
public:
	CollisionManager(GameObject** geometry, int);
	~CollisionManager() = default;
	
	int CheckSphereOnMeshes(GameObject* sphere, VECTOR3& outNormal, GameObject**& collidedObjects);
private:
	GameObject** staticGeometry;
	int geometryNumber;
};

