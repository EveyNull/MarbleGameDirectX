#include "Scene.h"
#include "LevelLoader.h"
#include "MathHelper.h"
#include <cmath>
#include <string>
#include <vector>


Scene::~Scene()
{

}

Scene::Scene(HWND hWnd, ID3D11Device* device, int number, AudioManager* audio, UIManager* ui)
{
	sceneNumber = number;
	audioManager = audio;
	uiManager = ui;

	std::string newText = "Level " + std::to_string(sceneNumber);
	std::wstring wide = std::wstring(newText.begin(), newText.end());
	uiManager->UpdateText(wide.c_str());

	GameObject* camera = new GameObject();
	camera->AddCameraComponent();
	mainCamera = camera;
	mainCamera->GetCameraComponent()->SetRotation({ 10, 90, 0 });

	GameObject* cube1 = new GameObject();
	cube1->AddMeshComponent(hWnd, device);
	cube1->GetMeshComponent()->MakeCube(device, 1000.0f, 1000.0f, 1000.0f);
	cube1->SetPosition({ 0.0, -100.0, 0.0 });
	cube1->GetMeshComponent()->LoadTexture(device, L"skybox.png");
	skyBox = cube1;

	playerSphere = new GameObject();

	playerSphere->AddMeshComponent(hWnd, device);
	playerSphere->GetMeshComponent()->MakeSphere(device, 0.45f, 12);
	playerSphere->GetMeshComponent()->LoadTexture(device, L"earth.jpg");
	playerSphere->SetPosition({ 0.0, 10.0, 0.0 });
	playerSphere->AddRigidbody(1);
	playerSphere->AddMovementComponent(mainCamera->GetCameraComponent()->GetYawPtr());

	LevelLoader* levelLoader = new LevelLoader();

	ID3D11ShaderResourceView* cubeTexture;
	HRESULT result = CreateWICTextureFromFile(device, L"minecraftcube.jpg", NULL, &cubeTexture);;

	levelGeometry = levelLoader->LoadLevel(sceneNumber, geometryNumber);

	for (int i = 0; i < geometryNumber; ++i)
	{
		levelGeometry[i]->AddMeshComponent(hWnd, device);
		ObjectType type = levelGeometry[i]->GetObjectType();
		if (type == ObjectType::CUBE || type == ObjectType::GOAL)
		{
			levelGeometry[i]->GetMeshComponent()->MakeCube(device, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			levelGeometry[i]->GetMeshComponent()->MakeSlope(device, 1.0f, 1.0f, 1.0f);
			if ((int)type > 3)
			{
				levelGeometry[i]->SetRotation(XMMatrixRotationRollPitchYaw(0, (XM_PI / 180) * (90 * ((int)type - 3)), 0));
			}
		}
		if (type == ObjectType::GOAL)
		{
			levelGeometry[i]->GetMeshComponent()->LoadTexture(device, L"minecraftcubegoal.jpg");
		}
		else
		{
			levelGeometry[i]->GetMeshComponent()->LoadTexture(device, cubeTexture);
		}
	}

	collisionManager = new CollisionManager(levelGeometry, geometryNumber);

	GameObject* dirLight = new GameObject();
	dirLight->AddLightComponent({ 0.0f, -3.0f, 0.0f });
	directionalLight = dirLight;
}


int Scene::Update(float dt)
{
	mainCamera->Update(dt);
	VECTOR3 collisionNormal;
	VECTOR3 collisionPoint;
	VECTOR3 currentPosition = playerSphere->GetPosition();


	VECTOR3 currentVelocity = playerSphere->GetRigidbody()->GetVelocity();

	int attempts = 0;
	GameObject** collidedObjects = nullptr;

	bool changeLevel = false;
	int numCollisions = collisionManager->CheckSphereOnMeshes(playerSphere, collisionNormal, collidedObjects);
	while (numCollisions > 0)
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
			newVel.x * MathHelper::Lerp(1.0f, 0.6f, abs(collisionNormal.x)),
			newVel.y * MathHelper::Lerp(1.0f, 0.6f, abs(collisionNormal.y)),
			newVel.z * MathHelper::Lerp(1.0f, 0.6f, abs(collisionNormal.z))
		};


		playerSphere->GetRigidbody()->SetVelocity(newVel);
		playerSphere->SetPosition(playerSphere->GetPosition() + newVel * dt * 2);

		if (collisionNormal.y < 0.0f)
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
		else
		{
			for (int i = 0; i < numCollisions; ++i)
			{
				if (collidedObjects[i]->GetObjectType() == ObjectType::GOAL)
				{
					changeLevel = true;
				}
			}
		}
		numCollisions = collisionManager->CheckSphereOnMeshes(playerSphere, collisionNormal, collidedObjects);
	}
	playerSphere->SetPosition(playerSphere->GetPosition() + (playerSphere->GetRigidbody()->GetVelocity() * dt));

	playerSphere->Update(dt);

	if (falling)
	{
		respawnTimer += dt;
		if (respawnTimer > 3.0f)
		{
			playerSphere->SetPosition({ 0.0f, 1.0f, 0.0f });
			playerSphere->GetRigidbody()->SetVelocity({ 0.0f, 0.0f, 0.0f });
			falling = false;
			respawnTimer = 0.0f;
			if (uiManager->UpdateLives() <= 0)
			{
				return 0;
			}
		}
	}
	else
	{
		mainCamera->SetPosition(playerSphere->GetPosition());

		currentPosition = playerSphere->GetPosition();
		skyBox->SetPosition({ currentPosition.x, currentPosition.y - 100.0f, currentPosition.z });
		if (currentPosition.y < -25.0f || currentPosition.y > 10.0f)
		{
			falling = true;
			audioManager->PlayWav(2, -2000.0f, 25000.0);
		}
		if (changeLevel)
		{
			return sceneNumber + 1;
		}
	}
	return sceneNumber;
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