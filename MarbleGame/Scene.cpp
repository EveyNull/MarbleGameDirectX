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
	sphere->GetMeshComponent()->MakeSphere(device, 0.4f, 8);
	sphere->GetMeshComponent()->LoadTexture(device, L"braynzar.jpg");
	sphere->SetPosition({ 0.0, 2.0, 0.5 });
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
	dirLight->AddLightComponent({ 0.0f, -3.0f, 0.0f });
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

	int attempts = 0;
	while (collisionManager->CheckSphereOnMeshes(playerSphere, collisionNormal, collisionPoint))
	{
		collisionNormal = collisionNormal.normalise();
		VECTOR3 normVel = currentVelocity.normalise();

		float a = -2 * (normVel.x * collisionNormal.x + normVel.y * collisionNormal.y + normVel.z * collisionNormal.z);

		VECTOR3 newVel = collisionNormal;
		newVel *= a;
		newVel += normVel;

		newVel *= currentVelocity.magnitude() * 0.9f;

		playerSphere->GetRigidbody()->SetVelocity(newVel * 0.4f);
		playerSphere->SetPosition(playerSphere->GetPosition() + newVel * dt * 2);
		attempts++;
		if (attempts > 5)
		{
			playerSphere->SetPosition(playerSphere->GetPosition() + VECTOR3{0, 0.3f, 0});
			break;
		}
	}
	playerSphere->SetPosition(playerSphere->GetPosition() + (playerSphere->GetRigidbody()->GetVelocity() * dt));

	VECTOR3 cameraOffset = { 0.0, 1.0, -15.0 };
	mainCamera->SetPosition(playerSphere->GetPosition() + cameraOffset);

	if (playerSphere->GetPosition().y < -500.0f)
	{
		playerSphere->SetPosition({ 0.0f, 10.0f, 0.0f });
		playerSphere->GetRigidbody()->SetVelocity({ 0,0,0 });
	}
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