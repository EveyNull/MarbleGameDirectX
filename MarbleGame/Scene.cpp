#include "Scene.h"
#include "LevelLoader.h"
#include "MathHelper.h"
#include <vector>
#include <cmath>

Scene::~Scene()
{

}

Scene::Scene(HWND hWnd, ID3D11Device* device, int number)
{
	sceneNumber = number;

	GameObject* camera = new GameObject();
	camera->AddCameraComponent();
	mainCamera = camera;
	mainCamera->GetCameraComponent()->SetRotation({ 10, 90, 0 });

	GameObject* cube1 = new GameObject();
	cube1->AddMeshComponent(hWnd, device);
	cube1->GetMeshComponent()->MakeCube(device, 1000.0f, 1000.0f, 1000.0f);
	cube1->SetPosition({ 0.0, 0.0, 0.0 });
	cube1->GetMeshComponent()->LoadTexture(device, L"skybox.png");
	skyBox = cube1;

	playerSphere = new GameObject();

	playerSphere->AddMeshComponent(hWnd, device);
	playerSphere->GetMeshComponent()->MakeSphere(device, 0.45f, 12);
	playerSphere->GetMeshComponent()->LoadTexture(device, L"braynzar.jpg");
	playerSphere->SetPosition({ 0.0, 10.0, 0.0 });
	playerSphere->AddRigidbody(1);
	playerSphere->AddMovementComponent(mainCamera->GetCameraComponent()->GetYawPtr());

	LevelLoader* levelLoader = new LevelLoader();

	levelGeometry = levelLoader->LoadLevel(sceneNumber, geometryNumber);

	for (int i = 0; i < geometryNumber; ++i)
	{
		levelGeometry[i]->AddMeshComponent(hWnd, device);
		levelGeometry[i]->GetMeshComponent()->MakeCube(device, 1.0f, 1.0f, 1.0f);
		levelGeometry[i]->GetMeshComponent()->LoadTexture(device, L"minecraftcube.jpg");
	}

	collisionManager = new CollisionManager(levelGeometry, geometryNumber);

	GameObject* dirLight = new GameObject();
	dirLight->AddLightComponent({ 0.0f, -3.0f, 0.0f });
	directionalLight = dirLight;
}


bool Scene::Update(float dt)
{
	mainCamera->Update(dt);
	VECTOR3 collisionNormal;
	VECTOR3 collisionPoint;
	VECTOR3 currentPosition = playerSphere->GetPosition();


	VECTOR3 currentVelocity = playerSphere->GetRigidbody()->GetVelocity();

	int attempts = 0;
	while (collisionManager->CheckSphereOnMeshes(playerSphere, collisionNormal, collisionPoint))
	{
		collisionNormal = collisionNormal.normalise();
		VECTOR3 normVel = currentVelocity.normalise();

		float dot = -2 * (normVel.x * collisionNormal.x + normVel.y * collisionNormal.y + normVel.z * collisionNormal.z);

		VECTOR3 newVel = collisionNormal;
		newVel *= dot;
		newVel += normVel;

		newVel *= currentVelocity.magnitude();

		newVel =
		{
			newVel.x * MathHelper::Lerp(1.0f, 0.4f, abs(collisionNormal.x)),
			newVel.y * MathHelper::Lerp(1.0f, 0.4f, abs(collisionNormal.y)),
			newVel.z * MathHelper::Lerp(1.0f, 0.4f, abs(collisionNormal.z))
		};


		playerSphere->GetRigidbody()->SetVelocity(newVel);
		playerSphere->SetPosition(playerSphere->GetPosition() + newVel * dt * 2);

		if (collisionNormal.y == -1)
		{
			playerSphere->GetMoveComponent()->SetCanJump(true);
		}

		attempts++;
		if (attempts > 1)
		{
			playerSphere->SetPosition(playerSphere->GetPosition() - collisionNormal * 0.01f);
			if (attempts > 50)
			{
				break;
			}
		}
	}
	playerSphere->SetPosition(playerSphere->GetPosition() + (playerSphere->GetRigidbody()->GetVelocity() * dt));

	playerSphere->Update(dt);

	mainCamera->SetPosition(playerSphere->GetPosition());

	if (playerSphere->GetPosition().y < -25.0f)
	{
		return true;
	}
	return false;
}

void Scene::Render(Renderer* renderer)
{
	std::vector<MeshComponent*> meshes;
	meshes.push_back(skyBox->GetMeshComponent());
	meshes.push_back(playerSphere->GetMeshComponent());

	for (int i = 0; i < geometryNumber; ++i)
	{
		meshes.push_back(levelGeometry[i]->GetMeshComponent());
	}

	renderer->Render(mainCamera->GetCameraComponent(), meshes, directionalLight->GetLightComponent());
}