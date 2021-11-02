#include "Scene.h"
#include <vector>
Scene::~Scene()
{
	for (GameObject* object : gameObjects)
	{
		delete object;
	}
}

Scene::Scene(HWND hWnd, ID3D11Device* device)
{
	GameObject* cube1 = new GameObject();
	cube1->AddMeshComponent(hWnd, device);
	cube1->GetMeshComponent()->MakeCube(device, 1000.0f, 1000.0f, 1000.0f);
	cube1->SetPosition({ 0.0, 0.0, 0.0 });
	cube1->GetMeshComponent()->LoadTexture(device, L"skybox.jpg");
	gameObjects.push_back(cube1);

	GameObject* cube2 = new GameObject();
	cube2->AddMeshComponent(hWnd, device);
	cube2->GetMeshComponent()->MakeCube(device, 1, 1, 1);
	cube2->GetMeshComponent()->LoadTexture(device, L"braynzar.jpg");
	cube2->SetPosition({ 1.0, 1.0, 1.0 });
	gameObjects.push_back(cube2);

	GameObject* camera = new GameObject();
	camera->AddCameraComponent();
	mainCamera = camera;

	GameObject* dirLight = new GameObject();
	dirLight->AddLightComponent({ 1.0f, 1.0f, 1.0f });
	directionalLight = dirLight;
}


void Scene::Update(float dt)
{
	mainCamera->Update(dt);
	for (GameObject* object : gameObjects)
	{
		object->Update(dt);
	}
}

void Scene::Render(Renderer* renderer)
{
	std::vector<MeshComponent*> meshes;
	meshes.push_back(gameObjects[0]->GetMeshComponent());
	meshes.push_back(gameObjects[1]->GetMeshComponent());
	renderer->Render(mainCamera->GetCameraComponent(), meshes, directionalLight->GetLightComponent());
}