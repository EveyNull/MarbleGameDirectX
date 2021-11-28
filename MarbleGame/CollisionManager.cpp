#include "CollisionManager.h"
#include <math.h>
#include <iostream>
#include <fstream>

float Dot(const XMVECTOR& a, const XMVECTOR& b);

CollisionManager::CollisionManager(GameObject** geometry, int geometryCount)
{
	staticGeometry = geometry;
	geometryNumber = geometryCount;
}

int CollisionManager::CheckSphereOnMeshes(GameObject* sphere, VECTOR3& outNormal, GameObject**& collidedObjects)
{
	outNormal = { 0,0,0 };
	float sphereRadius = sphere->GetMeshComponent()->GetVertices()[0].position.y;
	VECTOR3 spherePosV3 = sphere->GetPosition();
	XMVECTOR spherePos = { spherePosV3.x, spherePosV3.y, spherePosV3.z };
	int numCollisions = 0;

	for (int i = 0; i < geometryNumber; ++i)
	{
		int trisCollided = 0;

		MeshComponent* mesh = staticGeometry[i]->GetMeshComponent();
		unsigned long* indices = mesh->GetIndices();
		VertexConfig* vertices = mesh->GetVertices();
		for (int j = 0; j < mesh->GetIndexCount(); j += 3)
		{
			VertexConfig tri[3] = {
				vertices[indices[j]],
				vertices[indices[j+1]],
				vertices[indices[j+2]]
			};

			// Separating axis testing from https://realtimecollisiondetection.net/blog/?p=103
			bool collidedTri = false;

			XMMATRIX rotationMatrix = mesh->GetRotationMatrix();
			VECTOR3 objPosition = staticGeometry[i]->GetPosition();
			XMVECTOR xmPosition;
			xmPosition = { objPosition.x, objPosition.y, objPosition.z };
			
			XMVECTOR tri0, tri1, tri2;
			tri0 = { tri[0].position.x, tri[0].position.y, tri[0].position.z };
			tri1 = { tri[1].position.x, tri[1].position.y, tri[1].position.z };
			tri2 = { tri[2].position.x, tri[2].position.y, tri[2].position.z };

			XMVECTOR point1 = XMVector3TransformCoord(tri0, rotationMatrix) + xmPosition;
			XMVECTOR point2 = XMVector3TransformCoord(tri1, rotationMatrix) + xmPosition;
			XMVECTOR point3 = XMVector3TransformCoord(tri2, rotationMatrix) + xmPosition;

			VECTOR3 v1, v2, v3;
			XMFLOAT3 temp1, temp2, temp3;
			XMStoreFloat3(&temp1, point1 - spherePos);
			XMStoreFloat3(&temp2, point2 - spherePos);
			XMStoreFloat3(&temp3, point3 - spherePos);
			v1 = { temp1.x, temp1.y, temp1.z };
			v2 = { temp2.x, temp2.y, temp2.z };
			v3 = { temp3.x, temp3.y, temp3.z };

			XMVECTOR cross1 = { (v2 - v1).x, (v2 - v1).y, (v2 - v1).z };
			XMVECTOR cross2 = { (v3 - v1).x, (v3 - v1).y, (v3 - v1).z };

			XMVECTOR a = { v1.x, v1.y, v1.z };
			XMVECTOR crossProduct = DirectX::XMVector3Cross(cross1, cross2);
			XMVECTOR normal = DirectX::XMVector3Normalize(crossProduct);
			float distance = abs(Dot(a, normal));

			bool separate = distance > sphereRadius;
			if (separate) continue;

			XMVECTOR b = { v2.x, v2.y, v2.z };
			XMVECTOR c = { v3.x, v3.y, v3.z };

			float aa, ab, ac,
				bb, bc,
				cc,
				r2;

			aa = Dot(a, a);
			ab = Dot(a, b);
			ac = Dot(a, c);
			bb = Dot(b, b);
			bc = Dot(b, c);
			cc = Dot(c, c);
			r2 = pow(sphereRadius, 2);

			separate = ((aa > r2) & (ab > aa) & (ac > aa))
				| ((bb > r2) & (ab > bb) & (bc > bb))
				| ((cc > r2) & (ac > cc) & (bc > cc));

			if (separate) continue;

			XMVECTOR AB = b - a;
			XMVECTOR BC = c - b;
			XMVECTOR CA = a - c;

			float d1 = Dot(a, AB);
			float e1 = Dot(AB, AB);
			float d2 = Dot(b, BC);
			float e2 = Dot(BC, BC);
			float d3 = Dot(c, CA);
			float e3 = Dot(CA, CA);

			XMVECTOR Q1 = a * e1 - d1 * AB;
			XMVECTOR QC = c * e1 - Q1;
			XMVECTOR Q2 = b * e2 - d2 * BC;
			XMVECTOR QA = a * e2 - Q2;
			XMVECTOR Q3 = c * e3 - d3 * CA;
			XMVECTOR QB = b * e3 - Q3;

			bool separated1 = (Dot(Q1, Q1) > r2 * pow(e1, 2)) & (Dot(Q1, QC) > 0);
			bool separated2 = (Dot(Q2, Q2) > r2 * pow(e2, 2)) & (Dot(Q2, QA) > 0);
			bool separated3 = (Dot(Q3, Q3) > r2 * pow(e3, 2)) & (Dot(Q3, QB) > 0);

			separate = separated1 | separated2 | separated3;
			
			if (!separate)
			{
				XMFLOAT3 float3;
				XMStoreFloat3(&float3, normal);
				outNormal += { float3.x, float3.y, float3.z };

				numCollisions++;
				if (numCollisions == 1)
				{
					collidedObjects = new GameObject*[1];
					collidedObjects[0] = staticGeometry[i];
				}
				else
				{
					GameObject** buffer = new GameObject*[numCollisions];
					for (int i = 0; i < numCollisions -1; ++i)
					{
						buffer[i] = collidedObjects[i];
					}
					buffer[numCollisions - 1] = staticGeometry[i];
					delete[] collidedObjects;
					collidedObjects = buffer;
				}
			}
		}
	}
	if (numCollisions > 0)
	{
		if (numCollisions > 1)
		{
			outNormal /= numCollisions;
		}
		return numCollisions;
	}
	return 0;
}

float Dot(const XMVECTOR& a, const XMVECTOR& b)
{
	float output;
	XMStoreFloat(&output, XMVector3Dot(a, b));
	return output;
}