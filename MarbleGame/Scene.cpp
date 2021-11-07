#include "Scene.h"
#include "LevelLoader.h"
#include <vector>

float Dot(const VECTOR3& a, const VECTOR3& b);

Scene::~Scene()
{

}

Scene::Scene(HWND hWnd, ID3D11Device* device)
{
	GameObject* cube1 = new GameObject();
	cube1->AddMeshComponent(hWnd, device);
	cube1->GetMeshComponent()->MakeCube(device, 1000.0f, 1000.0f, 1000.0f);
	cube1->SetPosition({ 0.0, 0.0, 0.0 });
	cube1->GetMeshComponent()->LoadTexture(device, L"skybox.png");
	skyBox = cube1;

	GameObject* sphere = new GameObject();
	sphere->AddMeshComponent(hWnd, device);
	sphere->GetMeshComponent()->MakeSphere(device, 0.5f, 8);
	sphere->GetMeshComponent()->LoadTexture(device, L"braynzar.jpg");
	sphere->SetPosition({ 0.0, 2.0, 0.0 });
	sphere->AddRigidbody(1);
	playerSphere = sphere;

	LevelLoader* levelLoader = new LevelLoader();

	levelGeometry = levelLoader->LoadLevel(1, geometryNumber);

	for (int i = 0; i < geometryNumber; ++i)
	{
		levelGeometry[i]->AddMeshComponent(hWnd, device);
		levelGeometry[i]->GetMeshComponent()->MakeCube(device, 1.0f, 1.0f, 1.0f);
		levelGeometry[i]->GetMeshComponent()->LoadTexture(device, L"minecraftcube.jpg");
	}

	collisionManager = new CollisionManager(levelGeometry, geometryNumber);

	GameObject* camera = new GameObject();
	camera->AddCameraComponent();
	mainCamera = camera;

	GameObject* dirLight = new GameObject();
	dirLight->AddLightComponent({ 0.2f, -1.0f, 1.0f });
	directionalLight = dirLight;
}


void Scene::Update(float dt)
{
	mainCamera->Update(dt);
	VECTOR3 collisionNormal;
	VECTOR3 collisionPoint;
	VECTOR3 currentPosition = playerSphere->GetPosition();

	playerSphere->Update(dt);

	VECTOR3 currentVelocity = playerSphere->GetRigidbody()->GetVelocity();

	bool collision = collisionManager->CheckSphereOnMeshes(playerSphere, collisionNormal, collisionPoint);
	if (collision)
	{
		if (abs(collisionNormal.y == 0))
		{
			int test = 0;
		}

		playerSphere->GetRigidbody()->SetVelocity({
			abs(collisionNormal.x) > 0 ? 0 : currentVelocity.x,
			abs(collisionNormal.y) > 0 ? -currentVelocity.y : currentVelocity.y,
			abs(collisionNormal.z) > 0 ? 0 : currentVelocity.z,

			});
		VECTOR3 newVelocity = -collisionNormal * abs(Dot(playerSphere->GetRigidbody()->GetVelocity(), collisionNormal));
		playerSphere->GetRigidbody()->AddVelocity(newVelocity*1.75f);
		/*
		VECTOR3 ballVelocity = gameObjects[1]->GetRigidbody()->GetVelocity();
		XMVECTOR v1 = { collisionNormal.x, collisionNormal.y, collisionNormal.z };
		XMVECTOR v2 = { ballVelocity.x, ballVelocity.y, ballVelocity.z };
		XMFLOAT3 XMdot;
		XMStoreFloat3(&XMdot, DirectX::XMVector3Dot(v2, v1));
		VECTOR3 dot = { 0, XMdot.y, 0 };
		VECTOR3 newPosition = gameObjects[1]->GetPosition();
		gameObjects[1]->SetPosition({ newPosition.x, currentPosition.y, newPosition.z });
		gameObjects[1]->GetRigidbody()->AddVelocity((collisionNormal * dot) - (gameObjects[1]->GetRigidbody()->GetVelocity() * 1.5f));
		*/
	}
	playerSphere->SetPosition(playerSphere->GetPosition() + (playerSphere->GetRigidbody()->GetVelocity() * dt));

	VECTOR3 cameraOffset = { 0.0, 1.0, -15.0 };
	mainCamera->SetPosition(playerSphere->GetPosition() + cameraOffset);
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

float Dot(const VECTOR3& a, const VECTOR3& b)
{
	float output;
	XMVECTOR v1 = { a.x, a.y, a.z };
	XMVECTOR v2 = { b.x, b.y, b.z };
	XMStoreFloat(&output, XMVector3Dot(v1, v2));
	return output;
}