#pragma once
#include "CollisionManager.h"
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
	GameObject* skyBox;
	GameObject* mainCamera;
	GameObject* directionalLight;
	GameObject* playerSphere;

	GameObject** levelGeometry;
	int geometryNumber;

	CollisionManager* collisionManager;
};

