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
	GameObject* triangle = new GameObject();
	triangle->AddMeshComponent(hWnd, device);
	gameObjects.push_back(triangle);

	GameObject* triangle2 = new GameObject();
	triangle2->AddMeshComponent(hWnd, device);
	triangle2->SetPosition({ 5.0, 1.0, 0.0 });
	gameObjects.push_back(triangle2);

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