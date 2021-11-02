#pragma once
#include "GameObject.h"
#include "Renderer.h"
#include <vector>
class Scene
{
public:
	Scene(HWND hWnd, ID3D11Device* device);
	~Scene();
	void Update(float dt);
	void Render(Renderer*);
private:
	std::vector<GameObject*> gameObjects;

	GameObject* mainCamera;
	GameObject* directionalLight;
};

